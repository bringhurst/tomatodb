"""
Handle the control command.
"""

__author__     = "Jon Bringhurst <jon@bringhurst.org>"
__copyright__  = "Copyright 2013 Los Alamos National Security, LLC."
__license__    = "Apache License, Version 2.0"

class CommandControl():
    """Haven control command handler."""

    def handle(self, ctl, line):
        args = shlex.split(line)

        opts = [("attach",   HavenProtocol.HVN_CLNT_PROTO_CTRL_ATTACH),
                ("destroy",  HavenProtocol.HVN_CLNT_PROTO_CTRL_DESTROY),
                ("exit",     HavenProtocol.HVN_CLNT_PROTO_CTRL_EXIT),
                ("follower", HavenProtocol.HVN_CLNT_PROTO_CTRL_FOLLOWER),
                ("leader",   HavenProtocol.HVN_CLNT_PROTO_CTRL_LEADER),
                ("location", HavenProtocol.HVN_CLNT_PROTO_CTRL_LOCATION),
                ("proxy",    HavenProtocol.HVN_CLNT_PROTO_CTRL_PROXY),
                ("router",   HavenProtocol.HVN_CLNT_PROTO_CTRL_ROUTER)
        ]

        print("argv[0] = " + args[0])
        print("argv[1] = " + args[1])
        print("argv[2] = " + args[2])

        #follower<uuid>
        #leader<path>

        if ctl.conn.is_connected:
            print("Sending control message to `" + server + ":" + str(port) + "'.")
            ctl.proto.send_control(ctl.conn)
            ctl.proto.recv_control(ctl.conn)
        else:
            print("Connection to server failed.")

# EOF
