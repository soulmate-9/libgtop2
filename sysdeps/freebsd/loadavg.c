/* Copyright (C) 1998 Joshua Sled
   This file is part of LibGTop 1.0.

   Contributed by Joshua Sled <jsled@xcf.berkeley.edu>, July 1998.

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
#include <stdlib.h>
#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/loadavg.h>

#include <sys/types.h>
#include <sys/sysctl.h>

static const unsigned long _glibtop_sysdeps_loadavg =
(1L << GLIBTOP_LOADAVG_LOADAVG);

static const unsigned long _glibtop_sysdeps_last_pid =
(1L << GLIBTOP_LOADAVG_LAST_PID);

/* Init function. */

void
_glibtop_init_loadavg_s (glibtop *server)
{
	server->sysdeps.loadavg = _glibtop_sysdeps_loadavg |
		_glibtop_sysdeps_last_pid;
}

/* Provides load averange. */

void
glibtop_get_loadavg_s (glibtop *server, glibtop_loadavg *buf)
{
	double ldavg[3];
	pid_t last_pid;
	size_t len;
	int i;

	glibtop_init_s (&server, GLIBTOP_SYSDEPS_LOADAVG, 0);

	memset (buf, 0, sizeof (glibtop_loadavg));

	getloadavg (ldavg, 3);

	/* fill in the struct */
	buf->flags = _glibtop_sysdeps_loadavg;
	for (i = 0; i < 3; i++) {
		buf->loadavg [i] = ldavg [i];
	} /* end for */

	len = sizeof (last_pid);
	if (sysctlbyname ("kern.lastpid", &last_pid, &len, NULL, 0)) {
		glibtop_warn_io_r (server, "sysctl (kern.lastpid)");
		return;
	}

	buf->last_pid = last_pid;

	buf->flags |= _glibtop_sysdeps_last_pid;
}
