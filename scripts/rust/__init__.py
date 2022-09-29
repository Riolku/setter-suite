import sys

from .init_template import main as init_template_main
from .reload_library import main as reload_library_main


def main(args):
    assert len(args) >= 1, "Please specify a subcommand after `rust`"

    if args[0] == "init-template":
        return init_template_main(args[1:])

    elif args[0] == "reload-library":
        return reload_library_main(args[1:])

    print(f"Invalid sub-operation '{args[0]}'!", file=sys.stderr)
    return -1
