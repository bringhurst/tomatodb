AM_CPPFLAGS = \
              -I$(top_srcdir) \
              -I$(top_srcdir)/src/common/task

noinst_LTLIBRARIES = libtomato_common_task.la

libtomato_common_task_la_LDFLAGS = $(LIB_LDFLAGS)

libtomato_common_task_la_SOURCES = \
                                  asm.S \
                                  channel.c \
                                  context.c \
                                  fd.c \
                                  net.c \
                                  print.c \
                                  qlock.c \
                                  rendez.c \
                                  task.c
