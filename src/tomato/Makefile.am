include $(top_srcdir)/common.mk

AM_CPPFLAGS += \
               -I$(top_srcdir) \
               -I$(top_srcdir)/src/common

bin_PROGRAMS = tomato

tomato_SOURCES = \
                 cmd.c \
                 linenoise.c \
                 repl.c \
                 tomato.c

tomato_LDADD = ../common/libtomato_common.la
