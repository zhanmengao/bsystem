#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
const int fatal_signals[] = { SIGABRT,   SIGALRM, SIGBUS,  SIGFPE,  SIGHUP,  SIGILL,
	                          SIGINT,    SIGKILL, SIGQUIT, SIGSEGV, SIGSTOP, SIGTERM,
	                          SIGTSTP,   SIGTTIN, SIGTTOU, SIGUSR1, SIGUSR2,
#ifdef SIGPOLL
	                          SIGPOLL,
#endif
#ifdef SIGPROF
	                          SIGPROF,
#endif
#ifdef SIGSYS
	                          SIGSYS,
#endif
	                          SIGTRAP,
#ifdef SIGVTALRM
	                          SIGVTALRM,
#endif
	                          SIGXCPU,   SIGXFSZ };
static void fatal_handler(int signum)
{
	struct sigaction default_sigaction;
	sigset_t this_mask;
	printf("Caught signal '%s' [%d]", strsignal(signum), signum);

	if (terminal_needs_reset)
		tcsetattr(terminal_fildes, TCSAFLUSH, &orig_flags);

	default_sigaction.sa_handler = SIG_DFL;
	sigfillset(&(default_sigaction.sa_mask));
	default_sigaction.sa_flags = 0;
	sigaction(signum, &default_sigaction, NULL);
	sigemptyset(&this_mask);
	sigaddset(&this_mask, signum);
	pthread_sigmask(SIG_UNBLOCK, &this_mask, NULL);
	raise(signum);
}

int freerdp_handle_signals(void)
{
	size_t signal_index;
	sigset_t orig_set;
	struct sigaction orig_sigaction;
	struct sigaction fatal_sigaction;
	printf("Registering signal hook...");
	sigfillset(&(fatal_sigaction.sa_mask));
	sigdelset(&(fatal_sigaction.sa_mask), SIGCONT);
	pthread_sigmask(SIG_BLOCK, &(fatal_sigaction.sa_mask), &orig_set);
	fatal_sigaction.sa_handler = fatal_handler;
	fatal_sigaction.sa_flags = 0;

	for (signal_index = 0; signal_index < ARRAYSIZE(fatal_signals); signal_index++)
	{
		if (sigaction(fatal_signals[signal_index], NULL, &orig_sigaction) == 0)
		{
			if (orig_sigaction.sa_handler != SIG_IGN)
			{
				sigaction(fatal_signals[signal_index], &fatal_sigaction, NULL);
			}
		}
	}

	pthread_sigmask(SIG_SETMASK, &orig_set, NULL);
	/* Ignore SIGPIPE signal. */
	signal(SIGPIPE, SIG_IGN);
	return 0;
}