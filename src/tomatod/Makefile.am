include $(top_srcdir)/common.mk

AM_CPPFLAGS += \
               -I$(top_srcdir) \
               -I$(top_srcdir)/src/common

bin_PROGRAMS = tomatod

tomatod_SOURCES = \
                  attach.c \
                  bootstrap.c \
                  candidate.c \
                  consensus.c \
                  context.c \
                  follower.c \
                  tomatod.c \
                  hook.c \
                  leader.c \
                  replica.c \
                  routing.c \
                  settings.c \
                  timer.c

tomatod_LDADD = ../common/libtomato_common.la
