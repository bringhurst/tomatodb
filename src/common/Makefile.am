include $(top_srcdir)/common.mk

%.pb-c.c: %.proto
	protoc-c $< --c_out=.

SUBDIRS = halloc

INCLUDES = \
           -I$(top_srcdir) \
           -I$(top_srcdir)/src/common

noinst_LTLIBRARIES = libhaven_common.la

libhaven_common_la_LDFLAGS = $(LIB_LDFLAGS)

libhaven_common_la_SOURCES = \
							common.c \
							database.c \
							database.pb-c.c \
							admin.pb-c.c \
							client.pb-c.c \
							consensus.pb-c.c

libhaven_common_la_LIBADD = \
							halloc/libhalloc.a \
							$(protobuf-c_LIBS) \
							$(leveldb_LIBS) \
							$(uuid_LIBS) \
							$(zmq_LIBS)


libhaven_common_la_CFLAGS = \
							$(protobuf-c_CFLAGS) \
							$(leveldb_CFLAGS) \
							$(uuid_CFLAGS) \
							$(zmq_CFLAGS)
