include $(top_srcdir)/common.mk

AM_CPPFLAGS += \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/common

noinst_LTLIBRARIES = libtomato_common_xdr.la

msg_xdr.c:
	rpcgen -CN msg.x

libtomato_common_xdr_la_LDFLAGS = $(LIB_LDFLAGS)

libtomato_common_xdr_la_SOURCES = msg_xdr.c

libtomato_common_xdr_la_LIBADD =
libtomato_common_xdr_la_CFLAGS =

clean-local:
	-(rm -f msg.h msg_xdr.c)
