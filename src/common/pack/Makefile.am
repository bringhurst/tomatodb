include $(top_srcdir)/common.mk

AM_CPPFLAGS += \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/common \
           -I$(top_srcdir)/src/common/pack

noinst_LTLIBRARIES = libhaven_common_pack.la

libhaven_common_pack_la_LDFLAGS = $(LIB_LDFLAGS)

libhaven_common_pack_la_SOURCES = \
                                  addr_struct.c \
                                  connect_msg.c \
                                  control_msg.c \
                                  data_msg.c \
                                  discover_msg.c \
                                  heartbeat_msg.c \
                                  msgpack_helper.c \
                                  pack.c

libhaven_common_pack_la_LIBADD = \
                                 $(msgpack_LIBS) \
                                 $(uuid_LIBS)

libhaven_common_pack_la_CFLAGS = \
                                 $(msgpack_CFLAGS) \
                                 $(uuid_CFLAGS)
