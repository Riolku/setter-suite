from .executors import get_executor


def main(args):
    assert len(args) > 0, "Must provide the file to compile!"
    debug = False
    if "debug" in args:
        debug = True
        args.remove("debug")
    force_compile = False
    if "force" in args:
        force_compile = True
        args.remove("force")
    e = get_executor(args[0], args[1:], debug=debug, force_compile=force_compile)
    return 0
