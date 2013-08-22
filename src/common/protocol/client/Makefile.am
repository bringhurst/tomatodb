include $(top_srcdir)/common.mk

AM_CPPFLAGS += \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/common \
           -I$(top_srcdir)/src/common/protocol/client

noinst_LTLIBRARIES = libhaven_common_protocol_client.la

libhaven_common_protocol_client_la_LDFLAGS = $(LIB_LDFLAGS)

libhaven_common_protocol_client_la_SOURCES = \
                                             msgpack_helper.c \
                                             client_protocol.c \
                                             connect_msg.c \
                                             control_msg.c \
                                             data_msg.c \
                                             discover_msg.c \
                                             heartbeat_msg.c

libhaven_common_protocol_client_la_LIBADD = \
                                            $(msgpack_LIBS) \
                                            $(uuid_LIBS)

libhaven_common_protocol_client_la_CFLAGS = \
                                            $(msgpack_CFLAGS) \
                                            $(uuid_CFLAGS)
