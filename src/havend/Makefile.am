include $(top_srcdir)/common.mk

SUBDIRS = task state

INCLUDES = \
		   -I$(top_srcdir) \
		   -I$(top_srcdir)/src/common

bin_PROGRAMS = havend

havend_SOURCES = \
		 connection.c \
		 bootstrap.c \
		 settings.c \
		 consensus.c \
		 server.c \
		 havend.c

havend_LDADD = \
		../common/libhaven_common.la \
                state/libhavend_state.la \
		task/libtask.a
