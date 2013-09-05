AM_CPPFLAGS = \
              -I$(top_srcdir) \
              -I$(top_srcdir)/src/common/task

noinst_LTLIBRARIES = libhaven_common_task.la

libhaven_common_task_la_LDFLAGS = $(LIB_LDFLAGS)

libhaven_common_task_la_SOURCES = \
                                  asm.S \
                                  channel.c \
                                  context.c \
                                  fd.c \
                                  net.c \
                                  print.c \
                                  qlock.c \
                                  rendez.c \
                                  task.c \
                                  timer.c
