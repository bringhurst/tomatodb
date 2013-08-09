import socket
import sys

if not socket.error:
    socket.error = OSError

class HavenConnection():

    def __init__(self):
        self.s = None
        self.is_connected = False

    def connect(self, server, port):
        self.addr = server
        self.port = port
        self.is_connected = False

        try:
            self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.s.connect((self.addr, self.port))
            self.is_connected = True
        except OSError, msg:
            self.s = None
            self.is_connected = False

        if self.s is None:
            self.is_connected = False

    def disconnect(self):
        if self.s:
          self.s.close()
        self.is_connected = False
