include $(top_srcdir)/common.mk

AM_CPPFLAGS += \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/common

noinst_LTLIBRARIES = libtomato_common_proto.la

proto_xdr.c:
	rpcgen -CN proto.x

libtomato_common_proto_la_LDFLAGS = $(LIB_LDFLAGS)

libtomato_common_proto_la_SOURCES = \
                                    proto_xdr.c \
                                    socket.c

libtomato_common_proto_la_LIBADD =
libtomato_common_proto_la_CFLAGS =

clean-local:
	-(rm -f proto.h proto_xdr.c)
