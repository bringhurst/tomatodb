import readline
import cmd

from connection import HavenConnection

PROMPT_DISCONNECTED = "havenctl([NOT CONNECTED])> "

class HavenCtl(cmd.Cmd):
    """Haven daemon controller REPL."""

    def __init__(self):
        cmd.Cmd.__init__(self)
        self.prompt = PROMPT_DISCONNECTED
        self.intro  = "Haven daemon controller (0.0.1-alpha.1). Type help or ? to list commands."
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

    def do_connect(self, server=None, port=7854):
        """
        connect <server> [port]
            Connect to the specified server and port (default 7854).
        """
        if server:
            if self.conn:
                self.conn.disconnect()
            self.prompt = PROMPT_DISCONNECTED

            if not port:
                port = 7854

            print("Attempting to establish a connection to `" + server + ":" + str(port) + "'.")
            self.conn = HavenConnection()
            self.conn.connect(server, port)

            if self.conn.is_connected():
                print("Connection established.")
                self.prompt = "havenctl(" + server + ":" + str(port) + ")> "
            else:
                print("Connection to server failed.")
        else:
            print("You must specify a server with the connect command. Please see 'help connect'.")

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

def execute_from_command_line():
    HavenCtl().cmdloop()
