
INCLUDES			= @INCLUDES@

noinst_LTLIBRARIES		= libgtop_sysdeps-2.0.la libgtop_sysdeps_suid-2.0.la

libgtop_sysdeps_2_0_la_SOURCES	= nosuid.c siglist.c sysinfo.c shm_limits.c \
				  cpu.c msg_limits.c sem_limits.c loadavg.c \
				  uptime.c netlist.c fsusage.c mem.c \
				  mountlist.c procopenfiles.c procwd.c \
				  procaffinity.c glibtop_private.c open.c

libgtop_sysdeps_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)

libgtop_sysdeps_suid_2_0_la_LIBADD	= $(KVM_LIBS) $(EXTRA_SYSDEPS_LIBS)
libgtop_sysdeps_suid_2_0_la_SOURCES	= suid_open.c close.c swap.c \
				  proclist.c procstate.c procuid.c \
				  proctime.c procmem.c procsignal.c \
				  prockernel.c procsegment.c procargs.c \
				  procmap.c netload.c ppp.c

libgtop_sysdeps_suid_2_0_la_LDFLAGS	= $(LT_VERSION_INFO)

libgtopinclude_HEADERS		= glibtop_server.h glibtop_machine.h \
				  glibtop_suid.h glibtop_private.h

libgtopincludedir		= $(includedir)/libgtop-2.0
