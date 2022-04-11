from .executors import Executor


def main(args):
    assert len(args) > 0, "Must provide the file to compile!"
    e = Executor(args[0])
    return 0
