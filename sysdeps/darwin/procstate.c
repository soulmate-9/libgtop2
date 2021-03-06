/*
   This file is part of LibGTop 2.0.

   LibGTop is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   LibGTop is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with LibGTop; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <config.h>
#include <unistd.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/procstate.h>

#include <glibtop_suid.h>

#include <sys/param.h>
#include <sys/sysctl.h>      

static const unsigned long _glibtop_sysdeps_proc_state =
(1L << GLIBTOP_PROC_STATE_CMD) + (1L << GLIBTOP_PROC_STATE_STATE) +
(1L << GLIBTOP_PROC_STATE_UID) + (1L << GLIBTOP_PROC_STATE_GID);

/* Init function. */

void
_glibtop_init_proc_state_p (glibtop *server)
{
	server->sysdeps.proc_state = _glibtop_sysdeps_proc_state;
}

/* Provides detailed information about a process. */

void
glibtop_get_proc_state_p (glibtop *server, glibtop_proc_state *buf,
			  pid_t pid)
{
	int mib[4];
	size_t length;
	struct kinfo_proc kinfo;

	glibtop_init_p (server, (1 << GLIBTOP_SYSDEPS_PROC_STATE), 0);
	
	memset (buf, 0, sizeof (glibtop_proc_state));

	mib [0] = CTL_KERN;
	mib [1] = KERN_PROC;
	mib [2] = KERN_PROC_PID;
	mib [3] = pid;

	length = sizeof (struct kinfo_proc);
	if (sysctl (mib, 4, &kinfo, &length, NULL, 0) < 0) {
		glibtop_warn_io_r (server, "sysctl (procstat)");
		return;
	}

	strncpy (buf->cmd, kinfo.kp_proc.p_comm, sizeof (buf->cmd) - 1);
	buf->cmd [sizeof (buf->cmd) - 1] = 0;
	buf->uid = kinfo.kp_eproc.e_pcred.p_svuid;
	buf->gid = kinfo.kp_eproc.e_pcred.p_svgid;
	switch (kinfo.kp_proc.p_stat) {
	case SIDL:
		buf->state = 0;
		break;
	case SRUN:
		buf->state = 'R';
		break;
	case SSLEEP:
		buf->state = 'S';
		break;
	case SSTOP:
		buf->state = 'T';
		break;
	case SZOMB:
		buf->state = 'Z';
		break;
	default:
		return;
	}
	buf->flags = _glibtop_sysdeps_proc_state;
}

