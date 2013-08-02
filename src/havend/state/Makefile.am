include $(top_srcdir)/common.mk

%.machine.c: %.rl
	ragel -o $@ $< 

INCLUDES = \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/common

noinst_LTLIBRARIES = libhavend_state.la

libhavend_state_la_LDFLAGS = $(LIB_LDFLAGS)

libhavend_state_la_SOURCES = \
			client.machine.c \
			idle.machine.c \
			quorum.machine.c

libhavend_state_la_LIBADD =
libhavend_state_la_CFLAGS =
