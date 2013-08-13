include $(top_srcdir)/common.mk

AM_CPPFLAGS += \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/havend \
           -I$(top_srcdir)/src/common

noinst_LTLIBRARIES = libhaven_havend_client_protocol.la

libhaven_havend_client_protocol_la_LDFLAGS = $(LIB_LDFLAGS)

libhaven_havend_client_protocol_la_SOURCES = \
											 msgpack_helper.c \
											 client_protocol.c \
											 connect_msg.c \
											 control_msg.c \
											 data_msg.c \
											 discover_msg.c \
											 heartbeat_msg.c

libhaven_havend_client_protocol_la_LIBADD = \
					$(msgpack_LIBS) \
					$(uuid_LIBS)


libhaven_havend_client_protocol_la_CFLAGS = \
					$(msgpack_CFLAGS) \
					$(uuid_CFLAGS)
