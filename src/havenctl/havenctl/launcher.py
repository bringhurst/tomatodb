import sys
import getopt

from ctl import HavenCtl

def print_version():
    import pkg_resources  # part of setuptools
    version = pkg_resources.require("havenctl")[0].version
    print(version)

def print_usage():
    print """
Usage: havenctl [-a <remote_address> | --address=<remote_address>]
                [-p <remote_port> | --port=<remote_port>] [-hv] cmd
    """

def execute_from_command_line():
    address = None
    port = 7854

    try:
        opts, args = getopt.getopt(sys.argv[1:], "a:p:hv", \
                ["address=", "port=", "version", "help"])

    except getopt.GetoptError, err:
        print str(err)
        usage()
        sys.exit(2)

    for o, a in opts:
        if o in ("-h", "--help"):
            print_usage()
            sys.exit()
        elif o in ("-v", "--version"):
            print_version()
            sys.exit()
        elif o in ("-a", "--address"):
            address = a
        elif o in ("-p", "--port"):
            port = a
        else:
            usage()
            assert False, "unhandled option"
            sys.exit(2)

    remaining = " ".join(args)
    handle_user_args(address, port, remaining)

def handle_user_args(address, port, cmd):
    repl = HavenCtl()

    if(address):
        repl.onecmd("connect " + str(address) + " " + str(port))

    if(cmd):
        # When in non-REPL mode, we connect automatically.
        if cmd.strip().startswith("connect"):
            return
        else:
            repl.onecmd(cmd)
    else:
        repl.cmdloop()
