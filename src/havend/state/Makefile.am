include $(top_srcdir)/common.mk

%_machine.c: %.rl
	ragel -G2 -C -o $@ $< 

%.dot: %.rl
	ragel -p -V $< -o $@

%.png: %.dot
	dot -Tpng $< > $@

png: client.png consensus.png newbie.png

INCLUDES = \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/common

noinst_LTLIBRARIES = libhavend_state.la

libhavend_state_la_LDFLAGS = $(LIB_LDFLAGS)

libhavend_state_la_SOURCES = \
			client_machine.c \
			consensus_machine.c \
			newbie_machine.c

libhavend_state_la_LIBADD =
libhavend_state_la_CFLAGS =

CLEANFILES = \
			 client_machine.c \
			 consensus_machine.c \
			 newbie_machine.c
