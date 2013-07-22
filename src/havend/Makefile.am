include $(top_srcdir)/common.mk

INCLUDES = \
		   -I$(top_srcdir) \
		   -I$(top_srcdir)/src/common

bin_PROGRAMS = havend

havend_SOURCES = \
				 settings.c \
				 consensus.c \
				 database.c \
				 rpc.c \
				 havend.c

havend_LDADD = \
				../common/libhaven_common.la
