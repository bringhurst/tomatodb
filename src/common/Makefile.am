include $(top_srcdir)/common.mk

SUBDIRS = halloc task xdr

AM_CPPFLAGS += \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/common

noinst_LTLIBRARIES = libtomato_common.la

libtomato_common_la_LDFLAGS = $(LIB_LDFLAGS)

libtomato_common_la_SOURCES = \
                             common.c \
                             database.c \
                             xtime.c

libtomato_common_la_LIBADD = \
                             halloc/libtomato_common_halloc.la \
                             task/libtomato_common_task.la \
                             xdr/libtomato_common_xdr.la \
                             $(leveldb_LIBS) \
                             $(uuid_LIBS)

libtomato_common_la_CFLAGS = \
                            $(leveldb_CFLAGS) \
                            $(uuid_CFLAGS)
