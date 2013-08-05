include $(top_srcdir)/common.mk

%.machine.c: %.rl
	ragel -M -C -o $@ $< 

%.dot: %.rl
	ragel -p -V $< -o $@

%.png: %.dot
	dot -Tpng $< > $@

png: client.png quorum.png newbie.png

INCLUDES = \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/common

noinst_LTLIBRARIES = libhavend_state.la

libhavend_state_la_LDFLAGS = $(LIB_LDFLAGS)

libhavend_state_la_SOURCES = \
			client.machine.c \
			newbie.machine.c \
			quorum.machine.c

libhavend_state_la_LIBADD =
libhavend_state_la_CFLAGS =

CLEANFILES = \
			 client.machine.c \
			 newbie.machine.c \
			 quorum.machine.c
