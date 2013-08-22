include $(top_srcdir)/common.mk

SUBDIRS = client_protocol task

AM_CPPFLAGS += \
	   -I$(top_srcdir) \
	   -I$(top_srcdir)/src/common

bin_PROGRAMS = havend

havend_SOURCES = \
		 routing.c \
		 connection.c \
		 settings.c \
		 server.c \
		 havend.c

havend_LDADD = \
		../common/libhaven_common.la \
                client_protocol/libhaven_havend_client_protocol.la \
		task/libhaven_havend_task.la
