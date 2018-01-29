/*
 * Check printing of character/block device numbers with strace -yy.
 *
 * Copyright (c) 2018 The strace developers.
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

#include "tests.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>

#ifdef O_PATH

int
main(void)
{
	static const struct {
		const char *path;
		unsigned int major;
		unsigned int minor;
		bool blk;
		bool optional;
	} checks[] = {
		{ "/dev/zero", 1, 5, false, false },
		{ "/dev/full", 1, 7, false, false },
		{ "/dev/sda",  8, 0, true,  true  },
	};

	for (unsigned int i = 0; i < ARRAY_SIZE(checks); i++) {
		int fd = openat(AT_FDCWD, checks[i].path,
				O_RDONLY|O_LARGEFILE|O_PATH);

		printf("openat(AT_FDCWD, \"%s\", O_RDONLY|O_LARGEFILE|O_PATH)"
		       " = %s",
		       checks[i].path, sprintrc(fd));
		if (fd >= 0)
			printf("<%s:[%s %u:%u]>",
			       checks[i].path,
			       checks[i].blk ? "block" : "char",
			       checks[i].major, checks[i].minor);
		puts("");

		if (fd < 0) {
			if (checks[i].optional)
				continue;
			else
				perror_msg_and_fail("openat(\"%s\")",
						    checks[i].path);
		}

		int rc = fsync(fd);

		printf("fsync(%d<%s:[%s %u:%u]>) = %s\n",
		       fd, checks[i].path, checks[i].blk ? "block" : "char",
		       checks[i].major, checks[i].minor, sprintrc(rc));

		close(fd);
	}

	puts("+++ exited with 0 +++");

	return 0;
}

#else

SKIP_MAIN_UNDEFINED("O_PATH")

#endif
