include $(top_srcdir)/common.mk

AM_CPPFLAGS = -I$(top_srcdir)/src/common

%.pb-c.c: %.proto
	protoc-c $< --c_out=.

bin_PROGRAMS = havend havenctl

havenctl_SOURCES = \
				   admin.pb-c.c \
				   havenctl_peer.c \
				   havenctl.c

havenctl_LDADD = \
	$(protobuf-c_LIBS) \
	$(leveldb_LIBS)

havenctl_CPPFLAGS = \
    $(protobuf-c_CFLAGS) \
    $(leveldb_CFLAGS)

havend_SOURCES = \
				admin.pb-c.c \
				database.pb-c.c \
				client.pb-c \
				consensus.pb-c.c \
                consensus.c \
				database.c \
				rpc.c \
				havend.c

havend_LDADD = \
	$(protobuf-c_LIBS) \
	$(leveldb_LIBS)

havend_CPPFLAGS = \
    $(protobuf-c_CFLAGS) \
    $(leveldb_CFLAGS)
