"""
Handle the data command.
"""

__author__     = "Jon Bringhurst <jon@bringhurst.org>"
__copyright__  = "Copyright 2013 Los Alamos National Security, LLC."
__license__    = "Apache License, Version 2.0"

import shlex

from ..protocol import HavenProtocol

class CommandData():
    """Haven data command handler."""

    def handle(self, ctl, line):
        args = shlex.split(line)
        key = None
        value = None
        mode = None
        earliest = None
        latest = None

        if len(args) < 2:
            print("The data command requires arguments. Please see 'help data'.")
            return

        opts = {
            "read":        HavenProtocol.HVN_CLNT_PROTO_DATA_VERB_READ,
            "write":       HavenProtocol.HVN_CLNT_PROTO_DATA_VERB_WRITE,
            "delete":      HavenProtocol.HVN_CLNT_PROTO_DATA_VERB_DELETE,
            "watch":       HavenProtocol.HVN_CLNT_PROTO_DATA_VERB_WATCH,
            "unwatch":     HavenProtocol.HVN_CLNT_PROTO_DATA_VERB_UNWATCH,
            "transaction": HavenProtocol.HVN_CLNT_PROTO_DATA_TRANSACTION
        }

        if args[0].lower() in opts.keys():
            protocol_key = opts[args[0].lower()]
        else:
            print("Command `" + str(args[0]) + "' not recognized.")
            print("Please see 'help data'.")
            return

        if protocol_key == HavenProtocol.HVN_CLNT_PROTO_DATA_VERB_WRITE:
            if len(args) < 3:
                print("The write subcommand requires a key and value argument.")
                print("Please see 'help data'.")
                return
            key = args[1]
            value = args[2]

        if ctl.conn.is_connected:
            ctl.proto.send_data(ctl.conn, protocol_key, mode, earliest, latest, key, value):
            ctl.proto.recv_data(ctl.conn)
        else:
            print("Connection to server failed.")

# EOF
