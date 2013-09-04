include $(top_srcdir)/common.mk

AM_CPPFLAGS += \
               -I$(top_srcdir) \
               -I$(top_srcdir)/src/common

bin_PROGRAMS = havend

havend_SOURCES = \
                 attach.c \
                 bootstrap.c \
                 consensus.c \
                 context.c \
                 havend.c \
                 replica.c \
                 routing.c \
                 settings.c \
                 timer.c

havend_LDADD = ../common/libhaven_common.la
