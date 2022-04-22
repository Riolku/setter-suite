from .executors import Executor


def main(args):
    assert len(args) > 0, "Must provide the file to compile!"
    e = Executor(args[0], debug="debug" in args, force_compile="force" in args)
    return 0
