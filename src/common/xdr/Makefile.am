include $(top_srcdir)/common.mk

AM_CPPFLAGS += \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/common

noinst_LTLIBRARIES = libtomato_common_xdr.la

proto_xdr.c:
	rpcgen -CN proto.x

libtomato_common_xdr_la_LDFLAGS = $(LIB_LDFLAGS)

libtomato_common_xdr_la_SOURCES = proto_xdr.c

libtomato_common_xdr_la_LIBADD =
libtomato_common_xdr_la_CFLAGS =

clean-local:
	-(rm -f proto.h proto_xdr.c)
