import socket
import sys

if not socket.error:
    socket.error = OSError

class HavenConnection():

    def __init__(self):
        self.s = None
        self.is_connected = False

    def __del__(self):
        self.disconnect()

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

    def recv(self, length):
        if not self.is_connected:
            return False
        return self.s.recv(length)

    def send(self, msg):
        if not self.is_connected:
            return False
        self.s.sendall(msg)

    def disconnect(self):
        if self.s:
            print("Disconnecting from `" + str(self.addr) + ":" + str(self.port) + "'.")
            self.s.close()
        self.is_connected = False
