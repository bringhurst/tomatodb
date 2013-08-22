include $(top_srcdir)/common.mk

SUBDIRS = client server

noinst_LTLIBRARIES = libhaven_common_protocol.la

libhaven_common_protocol_la_SOURCES =

libhaven_common_protocol_la_LIBADD = \
                                     server/libhaven_common_protocol_server.la \
                                     client/libhaven_common_protocol_client.la
