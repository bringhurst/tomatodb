"""
Wrap protocol constants.
"""

__author__     = "Jon Bringhurst <jon@bringhurst.org>"
__copyright__  = "Copyright 2013 Los Alamos National Security, LLC."
__license__    = "Apache License, Version 2.0"

import msgpack
import struct

class HavenProtocol():

    # Version info.
    HVN_CLIENT_PROTOCOL_VERSION          = 0x01
    HVN_CLIENT_PROTOCOL_MAGIC            = 0xDECAFBAD

    # Client request msg types.
    HVN_CLNT_PROTO_MSG_TYPE_CONNECT      = 0x01
    HVN_CLNT_PROTO_MSG_TYPE_CONTROL      = 0x03
    HVN_CLNT_PROTO_MSG_TYPE_DATA         = 0x02
    HVN_CLNT_PROTO_MSG_TYPE_DISCOVER     = 0x06
    HVN_CLNT_PROTO_MSG_TYPE_HEARTBEAT    = 0x07

    # Server response msg types.
    HVN_CLNT_PROTO_MSG_TYPE_CONNECT_R    = 0x05
    HVN_CLNT_PROTO_MSG_TYPE_CONTROL_R    = 0x04
    HVN_CLNT_PROTO_MSG_TYPE_DATA_R       = 0x0C
    HVN_CLNT_PROTO_MSG_TYPE_DISCOVER_R   = 0x0D
    HVN_CLNT_PROTO_MSG_TYPE_HEARTBEAT_R  = 0x0F

    def send_connect(self, conn):
        msg = msgpack.packb([
            HavenProtocol.HVN_CLIENT_PROTOCOL_MAGIC,
            HavenProtocol.HVN_CLIENT_PROTOCOL_VERSION
        ])
        self.send(msg, conn)

    def recv_connect(self, conn):
        msg = self.recv(conn)
        print("Contents of received message: " + str(msgpack.unpackb(msg)))

    def send_control(self, conn):
        print "not implemented"

    def send_data(self, conn):
        print "not implemented"

    def send_discover(self, conn):
        print "not implemented"

    def send_heartbeat(self, conn):
        print "not implemented"

    def recv(self, conn):
        msg_len = struct.unpack("<L", conn.recv(4))[0]
        msg = conn.recv(msg_len)
        print("Received message of `{0}' (`{1}+4' bytes).".format(repr(msg), len(msg)))
        return msg

    def send(self, msg, conn):
        print("Sending message `{0}' (`{1}+4' bytes).".format(repr(msg), len(msg)))
        conn.send(struct.pack('!i', len(msg)))
        conn.send(msg)
