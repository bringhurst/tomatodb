"""
Handle the connect command.
"""

__author__     = "Jon Bringhurst <jon@bringhurst.org>"
__copyright__  = "Copyright 2013 Los Alamos National Security, LLC."
__license__    = "Apache License, Version 2.0"

import shlex

from ..connection import HavenConnection

class CommandConnect():
    """Haven connect command handler."""

    def handle(self, ctl, line):
        args = shlex.split(line)
        server = None
        port = 7854

        if len(args) > 0:
            server = args[0]
        if len(args) > 1:
            port = int(args[1])

        if server:
            if ctl.conn:
                ctl.conn.disconnect()

            ctl.prompt = ctl.PROMPT_DISCONNECTED
            ctl.conn = HavenConnection()
            ctl.conn.connect(server, port)

            if ctl.conn.is_connected:
                print("Connection established to `" + server + ":" + str(port) + "'.")
                ctl.proto.send_connect(ctl.conn)
                ctl.proto.recv_connect(ctl.conn)
                ctl.prompt = "havenctl(" + server + ":" + str(port) + ")> "
            else:
                print("Connection to server failed.")
        else:
            print("You must specify a server with the connect command. Please see 'help connect'.")

# EOF
