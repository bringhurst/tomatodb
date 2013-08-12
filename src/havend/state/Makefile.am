include $(top_srcdir)/common.mk

%_state.c: %.rl
	ragel -G2 -C -o $@ $< 

%.dot: %.rl
	ragel -p -V $< -o $@

%.png: %.dot
	dot -Tpng $< > $@

png: client.png consensus.png newbie.png

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src/common \
	-I$(top_srcdir)/src/havend

noinst_LTLIBRARIES = libhaven_havend_state.la

libhaven_havend_state_la_LDFLAGS = $(LIB_LDFLAGS)

libhaven_havend_state_la_SOURCES = consensus_state.c

libhaven_havend_state_la_LIBADD =
libhaven_havend_state_la_CFLAGS =
