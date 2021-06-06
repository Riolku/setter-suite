from .env import parse_env

from .countercase import main as countercase_main
from .generator import main as generator_main
from .init_template import main as init_template_main

def main(args):
    newargs, env = parse_env(args)

    if len(newargs) < 1:
        print("An operation is required.")
        return -1

    if newargs[0] == 'countercase':
        return countercase_main(newargs[1:], env)

    elif newargs[0] == 'generate':
        return generator_main(newargs[1:], env)

    elif newargs[0] == 'init-template':
        return init_template_main(newargs[1:], env)

    print(f"Invalid operation '{newargs[0]}'!")
    return -1
