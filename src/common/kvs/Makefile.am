include $(top_srcdir)/common.mk

AM_CPPFLAGS += \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/common \
           -I$(top_srcdir)/src/common/kvs

noinst_LTLIBRARIES = libtomato_common_kvs.la

libtomato_common_kvs_la_LDFLAGS = $(LIB_LDFLAGS)

libtomato_common_kvs_la_SOURCES = \
                                  kvs.c \
                                  kvs_leveldb.c

libtomato_common_kvs_la_LIBADD =
libtomato_common_kvs_la_CFLAGS =
