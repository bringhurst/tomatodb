"""
Handle the control command.
"""

__author__     = "Jon Bringhurst <jon@bringhurst.org>"
__copyright__  = "Copyright 2013 Los Alamos National Security, LLC."
__license__    = "Apache License, Version 2.0"

import shlex

from ..protocol import HavenProtocol

class CommandControl():
    """Haven control command handler."""

    def handle(self, ctl, line):
        args = shlex.split(line)
        uuid = None
        path = None

        if len(args) < 1:
            print("The control command requires arguments. Please see 'help control'.")
            return

        opts = {
            "attach":   HavenProtocol.HVN_PROTO_CTRL_ATTACH_DATA,
            "destroy":  HavenProtocol.HVN_PROTO_CTRL_DESTROY,
            "exit":     HavenProtocol.HVN_PROTO_CTRL_EXIT,
            "follower": HavenProtocol.HVN_PROTO_CTRL_FOLLOWER,
            "leader":   HavenProtocol.HVN_PROTO_CTRL_LEADER,
            "location": HavenProtocol.HVN_PROTO_CTRL_LOCATION,
            "proxy":    HavenProtocol.HVN_PROTO_CTRL_PROXY,
            "router":   HavenProtocol.HVN_PROTO_CTRL_ROUTER
        }

        if args[0].lower() in opts.keys():
            protocol_key = opts[args[0].lower()]
        else:
            print("Command `" + str(args[0]) + "' not recognized.")
            print("Please see 'help control'.")
            return

        if protocol_key == HavenProtocol.HVN_PROTO_CTRL_ATTACH_DATA:
            if len(args) < 2:
                print("The attach subcommand requires a uuid argument.")
                print("Please see 'help control'.")
                return
            uuid = args[1]

        if protocol_key == HavenProtocol.HVN_PROTO_CTRL_FOLLOWER:
            if len(args) < 2:
                print("The follower subcommand requires a uuid argument.")
                print("Please see 'help control'.")
                return
            uuid = args[1]

        if protocol_key == HavenProtocol.HVN_PROTO_CTRL_LEADER:
            if len(args) < 2:
                print("The leader subcommand requires a path argument.")
                print("Please see 'help control'.")
                return
            path = args[1]

        if ctl.conn.is_connected:
            ctl.proto.send_control(ctl.conn, protocol_key, uuid, path)
            ctl.proto.recv_control(ctl.conn)
        else:
            print("Connection to server failed.")

# EOF
