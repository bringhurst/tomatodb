include $(top_srcdir)/common.mk

AM_CPPFLAGS += \
           -Wno-unused-variable \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/common

PROTO_GEN = proto.h proto_xdr.c
$(PROTO_GEN) : proto-stamp
proto-stamp : proto.x
	rpcgen -CN proto.x
	touch $@

noinst_LTLIBRARIES = libtomato_common_proto.la

libtomato_common_proto_la_LDFLAGS = $(LIB_LDFLAGS)

libtomato_common_proto_la_SOURCES = \
                                    proto_xdr.c \
                                    stream.c

libtomato_common_proto_la_LIBADD =
libtomato_common_proto_la_CFLAGS =
