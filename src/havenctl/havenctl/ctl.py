import signal
import readline
import cmd
import shlex
import msgpack

from connection import HavenConnection
from protocol import HavenProtocol

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

    def do_connect(self, line):
        """
        connect <server> [port]
            Connect to the specified server and port (default 7854).
        """
        args = shlex.split(line)
        server = None
        port = 7854

        if len(args) > 0:
            server = args[0]
        if len(args) > 1:
            port = int(args[1])

        if server:
            if self.conn:
                self.conn.disconnect()

            self.prompt = PROMPT_DISCONNECTED
            self.conn = HavenConnection()
            self.conn.connect(server, port)
            self.proto.send_connect(self.conn)

            if self.conn.is_connected:
                print("Connection established to `" + server + ":" + str(port) + "'.")
                self.prompt = "havenctl(" + server + ":" + str(port) + ")> "
            else:
                print("Connection to server failed.")
        else:
            print("You must specify a server with the connect command. Please see 'help connect'.")

    def do_become(self, line):
        if self.conn is None or not self.conn.is_connected:
            print("You must be connected to a server to issue the become command.");
            return False

        become_type = 'location_leader'

        msg = {
            'action' : 'become',
            'type'   : become_type
        }

        packed_msg = msgpack.packb(msg)
        print("Sending message `{0}' (`{1}' bytes).".format(repr(packed_msg), len(packed_msg)))

        self.conn.send(packed_msg)

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
