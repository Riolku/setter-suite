from .env import env

from .countercase import main as countercase_main
from .generator import main as generator_main
from .init_template import main as init_template_main
from .init_yml import main as init_yml_main
from .build import main as build_main
from .judge import main as judge_main
from .invoke import main as invoke_main
from .validate import main as validate_main


def main(args):
    newargs = env.setup_env(args)

    if len(newargs) < 1:
        print("An operation is required.")
        return -1

    if newargs[0] == "countercase":
        return countercase_main(newargs[1:])

    elif newargs[0] == "generate":
        return generator_main(newargs[1:])

    elif newargs[0] == "init-template":
        return init_template_main(newargs[1:])

    elif newargs[0] == "init-yml":
        return init_yml_main(newargs[1:])

    elif newargs[0] == "build":
        return build_main(newargs[1:])

    elif newargs[0] == "judge":
        return judge_main(newargs[1:])

    elif newargs[0] == "invoke":
        return invoke_main(newargs[1:])

    elif newargs[0] == "validate":
        return validate_main(newargs[1:])

    print(f"Invalid operation '{newargs[0]}'!")
    return -1
