#ifndef FILE_H
#define FILE_H
#include<stdio.h>

typedef unsigned mode_t ;
typedef unsigned dev_t ;
typedef unsigned off_t;
//c 封装
struct inode
{
	int id;
};

struct file
{
    //member function
    int (*lseek) (struct inode *, struct file *, off_t, int);
    int (*read) (struct inode *, struct file *, char *, int);
    int (*write) (struct inode *, struct file *, char *, int);
    int (*readdir) (struct inode *, struct file *);
    int (*select) (struct inode *, struct file *);
    int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);
    int (*mmap) (struct inode *, struct file *, unsigned long, size_t, int, unsigned long);
    int (*open) (struct inode *, struct file *);
    void (*release) (struct inode *, struct file *);
    int (*fsync) (struct inode *, struct file *);

    //member data
    mode_t f_mode;
    dev_t f_rdev;			/* needed for /dev/tty */
    off_t f_pos;
    unsigned short f_flags;
    unsigned short f_count;
    unsigned short f_reada;
    struct file *f_next, *f_prev;
    struct inode * f_inode;
    //build
    void (*Init)(struct file* pThis);
};

int read(struct inode *i, struct file *f, char *buffer, int bfsz)
{
    printf("File Read \n");
}

void File_Init(struct file* pFile)
{
    pFile->read = read;
}

void ClassMain()
{
    struct file f;
    File_Init(&f);
    struct inode i;
    char buffer[1024] = {0};
    f.read(&i,&f,buffer,0);
}
#endif
