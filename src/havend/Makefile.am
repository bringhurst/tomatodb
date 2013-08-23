include $(top_srcdir)/common.mk

AM_CPPFLAGS += \
               -I$(top_srcdir) \
               -I$(top_srcdir)/src/common

bin_PROGRAMS = havend

havend_SOURCES = \
                 connection.c \
                 context.c \
                 havend.c \
                 proxy.c \
                 replica.c \
                 routing.c \
                 settings.c

havend_LDADD = ../common/libhaven_common.la
