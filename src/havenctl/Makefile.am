include $(top_srcdir)/common.mk

INCLUDES = \
		-I$(top_srcdir) \
		-I$(top_srcdir)/src/common

bin_PROGRAMS = haven

haven_SOURCES = \
				peer.c \
				haven.c

haven_LDADD = \
			../common/libhaven_common.la
