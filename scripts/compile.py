from .executors import get_executor


def main(args):
    assert len(args) > 0, "Must provide the file to compile!"
    e = get_executor(args[0], debug="debug" in args, force_compile="force" in args)
    return 0
