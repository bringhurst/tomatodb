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

    # Control client requests.
    HVN_CLNT_PROTO_CTRL_ATTACH           = 0x01
    HVN_CLNT_PROTO_CTRL_DESTROY          = 0x03
    HVN_CLNT_PROTO_CTRL_EXIT             = 0x02
    HVN_CLNT_PROTO_CTRL_FOLLOWER         = 0x06
    HVN_CLNT_PROTO_CTRL_LEADER           = 0x07
    HVN_CLNT_PROTO_CTRL_LOCATION         = 0x05
    HVN_CLNT_PROTO_CTRL_PROXY            = 0x04
    HVN_CLNT_PROTO_CTRL_ROUTER           = 0x0C

    # Control server responses.
    HVN_CLNT_PROTO_CTRL_R_OK             = 0x0D
    HVN_CLNT_PROTO_CTRL_R_ERR            = 0x1D
    HVN_CLNT_PROTO_CTRL_R_NOT_FOUND      = 0x1C

    def send_connect(self, conn):
        msg = msgpack.packb([
            HavenProtocol.HVN_CLNT_PROTO_MSG_TYPE_CONNECT,
            HavenProtocol.HVN_CLIENT_PROTOCOL_MAGIC,
            HavenProtocol.HVN_CLIENT_PROTOCOL_VERSION
        ])
        self.send(msg, conn)

    def recv_connect(self, conn):
        msg = self.recv(conn)
        print("Contents of received connect response message: " + \
            str(msgpack.unpackb(msg)))

    def send_control(self, conn, action, uuid=None, path=None):
        msg = msgpack.packb([
            HavenProtocol.HVN_CLNT_PROTO_MSG_TYPE_CONTROL,
            action,
            uuid,
            path
        ])
        self.send(msg, conn)

    def recv_control(self, conn):
        msg = self.recv(conn)
        print("Contents of received control response message: " + \
            str(msgpack.unpackb(msg)))

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
