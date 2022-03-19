#include <stdlib.h>
#include <pthread.h>

#define NHASH 29
#define HASH(id) (((unsigned long)id)%NHASH)

struct foo *fh[NHASH];
pthread_spinlock_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo {
	int             f_count; /* protected by hashlock */
	pthread_spinlock_t f_lock;
	int             f_id;
	struct foo     *f_next; /* protected by hashlock */
	/* ... more stuff here ... */
};

struct foo *foo_alloc(int id) /* allocate the object */
{
	struct foo	*fp;
	int			idx;

	if ((fp = malloc(sizeof(struct foo))) != NULL) {
		fp->f_count = 1;
		fp->f_id = id;
		if (pthread_spin_init(&fp->f_lock, NULL) != 0) {
			free(fp);
			return(NULL);
		}
		idx = HASH(id);
		pthread_spin_lock(&hashlock);
		fp->f_next = fh[idx];
		fh[idx] = fp;
		pthread_spin_lock(&fp->f_lock);
		pthread_spin_unlock(&hashlock);
		/* ... continue initialization ... */
		pthread_spin_unlock(&fp->f_lock);
	}
	return(fp);
}

void foo_hold(struct foo *fp) /* add a reference to the object */
{
	pthread_spin_lock(&hashlock);
	fp->f_count++;
	pthread_spin_unlock(&hashlock);
}

struct foo * foo_find(int id) /* find an existing object */
{
	struct foo	*fp;

	pthread_spin_lock(&hashlock);
	for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next) {
		if (fp->f_id == id) {
			fp->f_count++;
			break;
		}
	}
	pthread_spin_unlock(&hashlock);
	return(fp);
}

void foo_rele(struct foo *fp) /* release a reference to the object */
{
	struct foo	*tfp;
	int			idx;

	pthread_spin_lock(&hashlock);
	if (--fp->f_count == 0) { /* last reference, remove from list */
		idx = HASH(fp->f_id);
		tfp = fh[idx];
		if (tfp == fp) {
			fh[idx] = fp->f_next;
		} else {
			while (tfp->f_next != fp)
				tfp = tfp->f_next;
			tfp->f_next = fp->f_next;
		}
		pthread_spin_unlock(&hashlock);
		pthread_spin_destroy(&fp->f_lock);
		free(fp);
	} else {
		pthread_spin_unlock(&hashlock);
	}
}
