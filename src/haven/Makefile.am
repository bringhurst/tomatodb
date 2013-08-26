include $(top_srcdir)/common.mk

AM_CPPFLAGS += \
			   -D_FILE_OFFSET_BITS=64 \
               -I$(top_srcdir) \
               -I$(top_srcdir)/src/common

bin_PROGRAMS = haven

haven_SOURCES = \
                fuse_impl.c

haven_LDADD = ../common/libhaven_common.la
