/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

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
#include <sys/shm.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/shm_limits.h>

static const unsigned long _glibtop_sysdeps_shm_limits =
(1L << GLIBTOP_IPC_SHMMAX) + (1L << GLIBTOP_IPC_SHMMIN) +
(1L << GLIBTOP_IPC_SHMMNI);

/* Init function. */

void
_glibtop_init_shm_limits_p (glibtop *server)
{
	int result;

	result = _glibtop_get_kmem_offset(server, "shminfo");
	if (result == -1)
        {
		server->sysdeps.shm_limits = 0;
		return;
	}

	server->machine.shminfo_offset = result;

	server->sysdeps.shm_limits = _glibtop_sysdeps_shm_limits;
}

/* Provides information about sysv ipc limits. */

void
glibtop_get_shm_limits_p (glibtop *server, glibtop_shm_limits *buf)
{
	int result;
	struct shminfo shminfo;
	int s;

	glibtop_init_p (server, (1L << GLIBTOP_SYSDEPS_SHM_LIMITS), 0);

	memset (buf, 0, sizeof (glibtop_shm_limits));

	result = _glibtop_get_kmem_info(server, server->machine.shminfo_offset,
					&shminfo, sizeof(struct shminfo));
	if (result <= 0)
	{
		glibtop_error_io_r (server, "Cannot read shminfo");
		return;
	}

	buf->shmmax = shminfo.shmmax;
	buf->shmmin = shminfo.shmmin;
	buf->shmmni = shminfo.shmmni;

	buf->flags = _glibtop_sysdeps_shm_limits;
}
