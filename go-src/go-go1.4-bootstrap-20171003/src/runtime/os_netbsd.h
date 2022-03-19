// Copyright 2010 The Go Authors.  All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.


typedef uintptr kevent_udata;

struct sigaction;

void	runtime·sigpanic(void);

void	runtime·setitimer(int32, Itimerval*, Itimerval*);
void	runtime·sigaction(int32, struct sigaction*, struct sigaction*);
void	runtime·sigaltstack(SigaltstackT*, SigaltstackT*);
void	runtime·sigprocmask(int32, Sigset*, Sigset*);
void	runtime·unblocksignals(void);
int32	runtime·sysctl(uint32*, uint32, byte*, uintptr*, byte*, uintptr);
extern void runtime·lwp_tramp(void);

enum {
	SS_DISABLE = 4,
	SIG_BLOCK = 1,
	SIG_UNBLOCK = 2,
	SIG_SETMASK = 3,
	NSIG = 33,
	SI_USER = 0,

	// From NetBSD's <sys/ucontext.h>
	_UC_SIGMASK = 0x01,
	_UC_CPU = 0x04,
};
