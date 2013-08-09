from ctl import HavenCtl

def execute_from_command_line():
    begin_havenctl_repl()

def begin_havenctl_repl():
    HavenCtl().cmdloop()
