include $(top_srcdir)/common.mk

AM_CPPFLAGS = -I$(top_srcdir)/src/common

bin_PROGRAMS = haven

haven_SOURCES = \
				   haven.c

haven_LDADD = \
	$(protobuf-c_LIBS) \
	$(leveldb_LIBS)

haven_CPPFLAGS = \
    $(protobuf-c_CFLAGS) \
    $(leveldb_CFLAGS)
