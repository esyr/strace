#ifndef STRACE_GDBSERVER_H
#define STRACE_GDBSERVER_H

/* Interface of strace features over the GDB remote protocol.
 *
 * Copyright (c) 2015 Red Hat Inc.
 * Copyright (c) 2015 Josh Stone <cuviper@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "protocol.h"

#ifdef ENABLE_GDBSERVER
#include "gdb_arch_defs.h"

extern char *gdbserver;

#ifdef GDBSERVER_ARCH_HAS_GET_REGS
extern long gdb_get_regs(pid_t pid);
#else
static inline long gdb_get_regs(pid_t pid) { return -1; }
#endif

#ifdef GDBSERVER_ARCH_HAS_GET_REGS
extern int gdb_set_regs(pid_t pid);
#else
static inline int gdb_set_regs(pid_t pid) { return -1; }
#endif

extern int gdb_init(void);
extern void gdb_finalize_init(void);
extern void gdb_cleanup(void);
extern void gdb_detach(struct tcb *tcp);
extern void gdb_startup_child(char **argv);
extern void gdb_startup_attach(struct tcb *tcp);
extern bool gdb_trace(void);
extern char *gdb_recv_regs(pid_t tid, size_t *size);
extern int gdb_read_mem(pid_t tid, long addr, unsigned int len, bool check_nil,
			char *out);
extern int gdb_getfdpath(pid_t tid, int fd, char *buf, unsigned bufsize);
#else
/*
 * This definition makes command-line handling code in strace.c
 * a bit less cumbersome.
 */
# define gdbserver 0
#endif

#endif /* !STRACE_GDBSERVER_H */