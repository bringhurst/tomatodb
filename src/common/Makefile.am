include $(top_srcdir)/common.mk

SUBDIRS = halloc task proto kvs

AM_CPPFLAGS += \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/common

noinst_LTLIBRARIES = libtomato_common.la

libtomato_common_la_LDFLAGS = $(LIB_LDFLAGS)

libtomato_common_la_SOURCES = \
                             common.c \
                             xtime.c

libtomato_common_la_LIBADD = \
                             halloc/libtomato_common_halloc.la \
                             kvs/libtomato_common_kvs.la \
                             proto/libtomato_common_proto.la \
                             task/libtomato_common_task.la \
                             $(uuid_LIBS)

libtomato_common_la_CFLAGS = \
                            $(uuid_CFLAGS)
