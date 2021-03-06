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
#include <unistd.h>
#include <sys/vminfo.h>

#include <glibtop.h>
#include <glibtop/error.h>
#include <glibtop/mem.h>

static const unsigned long _glibtop_sysdeps_mem =
(1 << GLIBTOP_MEM_TOTAL) + (1 << GLIBTOP_MEM_USED) +
(1 << GLIBTOP_MEM_FREE) + (1 << GLIBTOP_MEM_LOCKED);

/* Init function. */

void
_glibtop_init_mem_s (glibtop *server)
{
	server->sysdeps.mem = _glibtop_sysdeps_mem;
}

/* Provides information about memory usage. */

void
glibtop_get_mem_s (glibtop *server, glibtop_mem *buf)
{
	struct vminfo vminfo;
	int pagesize;
	int result;

	glibtop_init_s (&server, (1L << GLIBTOP_SYSDEPS_MEM), 0);

	memset (buf, 0, sizeof (glibtop_mem));

	pagesize = sysconf(_SC_PAGESIZE);

#ifdef HAVE_VMGETINFO
	result = vmgetinfo((void*)&vminfo, VMINFO, sizeof(vminfo));
#else
	result = _glibtop_vmgetinfo((void*)&vminfo, VMINFO, sizeof(vminfo));
#endif
	if (result == -1)
	{
		glibtop_error_io_r (server, "Cannot read vminfo");
		return;
	}


	buf->total = vminfo.memsizepgs * pagesize;
	buf->used = (vminfo.numwseguse + vminfo.numpseguse +
		     vminfo.numclseguse) * pagesize;
	buf->free = vminfo.numfrb * pagesize;
	buf->locked = (vminfo.numwsegpin + vminfo.numpsegpin +
		       vminfo.numclsegpin) * pagesize;

	buf->flags = _glibtop_sysdeps_mem;
}
