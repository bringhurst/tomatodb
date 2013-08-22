include $(top_srcdir)/common.mk

SUBDIRS = halloc protocol task

AM_CPPFLAGS += \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/common

noinst_LTLIBRARIES = libhaven_common.la

libhaven_common_la_LDFLAGS = $(LIB_LDFLAGS)

libhaven_common_la_SOURCES = \
                             common.c \
                             database.c \
                             xtime.c

libhaven_common_la_LIBADD = \
                             halloc/libhaven_common_halloc.la \
                             task/libhaven_common_task.la \
                             protocol/libhaven_common_protocol.la \
                             $(leveldb_LIBS) \
                             $(uuid_LIBS)

libhaven_common_la_CFLAGS = \
                            $(leveldb_CFLAGS) \
                            $(uuid_CFLAGS)
