include $(top_srcdir)/common.mk

SUBDIRS = task

INCLUDES = \
		   -I$(top_srcdir) \
		   -I$(top_srcdir)/src/common

bin_PROGRAMS = havend

havend_SOURCES = \
		 settings.c \
		 consensus.c \
		 server.c \
		 havend.c

havend_LDADD = \
		../common/libhaven_common.la
		task/libtask.a

