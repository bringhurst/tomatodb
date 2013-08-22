include $(top_srcdir)/common.mk

AM_CPPFLAGS += \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/common \
           -I$(top_srcdir)/src/common/protocol/server

noinst_LTLIBRARIES = libhaven_common_protocol_server.la

libhaven_common_protocol_server_la_LDFLAGS = $(LIB_LDFLAGS)

libhaven_common_protocol_server_la_SOURCES = \
                                             rpc.c

libhaven_common_protocol_server_la_LIBADD = \
                                            $(uuid_LIBS)

libhaven_common_protocol_server_la_CFLAGS = \
                                            $(uuid_CFLAGS)
