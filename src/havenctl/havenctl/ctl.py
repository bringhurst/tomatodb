"""
Process user commands.
"""

__author__     = "Jon Bringhurst <jon@bringhurst.org>"
__copyright__  = "Copyright 2013 Los Alamos National Security, LLC."
__license__    = "Apache License, Version 2.0"

import signal
import readline
import cmd
import shlex
import msgpack

from connection import HavenConnection
from protocol import HavenProtocol

from cmds.command_connect import CommandConnect
from cmds.command_data import CommandData
from cmds.command_control import CommandControl
from cmds.command_discover import CommandDiscover
from cmds.command_heartbeat import CommandHeartbeat

PROMPT_DISCONNECTED = "havenctl([NOT CONNECTED])> "

class HavenCtl(cmd.Cmd):
    """Haven daemon controller REPL."""

    def __init__(self):
        cmd.Cmd.__init__(self)
        signal.signal(signal.SIGINT, self.sigint_handler)
        self.prompt = PROMPT_DISCONNECTED
        self.intro  = "\n ****\n" \
                      " **** Welcome to havenctl 0.0.1-alpha.1\n" \
                      " **** Type help or ? to list available commands.\n" \
                      " ****\n"
        self.proto = HavenProtocol()
        self.conn = None

    def do_connect(self, line):
        """
        connect <server> [port]
            Connect to the specified server and port (default 7854).
        """
        CommandConnect.handle(self, line)

    def do_control(self, line):
        """
        control <state> [args]
            Transition the remote server into the specified state and function.
       
            States:
                'exit [<destroy>]'  - Tell the remote server to exit. If destroy is 'true', delete the tablet.
                'follower <uuid>' - Follow the quorum (or lonely leader) with the specified uuid.
                'leader <path>'   - Create a new generic leader for the specified path.
                'location'        - Create a new location leader.
                'proxy'           - Become a proxy for end user client commands.
                'router'          - Become idle until transitioned into another state.
        """
        CommandControl.handle(self, line)

    def do_data(self, line):
        """
        data [<verb> | <transaction control>] [args]
            Perform actions related to stored keys and values.

            Verbs:
                'read <key> [<mode>]' - Read the data at key. Mode is required outside of transactions.
                'write <key> <value>' - In a transaction, write the value at the key location.
                'delete <key>' - In a transaction, delete the specified key.
                'watch <key>' - Begin watching the specified key value for changes.
                'unwatch <key>' - Stop watching the specified key.

            Transaction control:
                'transaction abort' - Abort the current transaction.
                'transaction commit' - Perform the current transaction.
                'transaction end' - End the current transaction if the commit buffer is empty.
                'transaction start <mode>' - Begin a transaction with the specified mode.

            Transaction modes:
                'read_write' - The transaction may include read and write operations.
                'read_only' - The transaction may only include read operations.

            Read modes:
                'read_bounded <age>' - Ensure data returned is newer than the specified age.
                'read_timestamp <earliest> <latest>' - Ensure data is bounded by the specified timestamps.
              
        """
        CommandData.handle(self, line)

    def do_discover(self, line):
        """
        discover <type> [<uuid> | <key>]
            Determine the location of resources.

            Types:
                'leader' - Return the leader address and UUID of the specified path or UUID.
                'location' - Return the quorum addresses and UUID of the primary location service.
                'quorum' - Return the quorum addresses and UUID of the specified path or UUID.
        """
        CommandDiscover.handle(self, line)

    def do_heartbeat(self, line):
        """
        heartbeat TODO
        """
        CommandHeartbeat.handle(self, line)

    def do_disconnect(self, line):
        """
        disconnect
            Disconnect from the current server.
        """
        if self.conn:
            self.conn.disconnect()
        else:
            print("Not currently connected to a server. Please see 'help disconnect'.")
        self.prompt = PROMPT_DISCONNECTED

    def do_EOF(self, line):
        print("\nEncountered EOF. Exiting Haven daemon controller. Goodbye!")
        return True

    def do_exit(self, line):
        print("Exiting Haven daemon controller. Goodbye!")
        return True

    def do_quit(self, line):
        print("Quitting Haven daemon controller. Goodbye!")
        return True

    def postloop(self):
        print

    def sigint_handler(self, signum, frame):
        return True
