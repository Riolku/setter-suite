import os

from .env import env


def main(args):
    if os.path.exists("old-build"):
        os.system("rm -r old-build")

    try:
        os.rename("build", "old-build")
    except FileNotFoundError:
        pass

    os.mkdir("build")

    link_config()
    link_init()
    link_data()
    link_checker()
    link_interactor()

    print("---Generated build folder---")
    return 0


def link_config():
    os.symlink(f"../config.yml", f"build/config.yml")


def link_init():
    os.symlink(f"../init.yml", f"build/init.yml")


def link_data():
    init_type = env.init_type

    if init_type == "zip":
        os.symlink("../data/data.zip", "build/data.zip")

    os.symlink(f"../{env.generator}", f"build/{env.generator}")


def link_checker():
    if "interactor" not in env and env.checker_name.endswith("cpp"):
        os.symlink(f"../{env.checker_name}", f"build/{env.checker_name}")


def link_interactor():
    if "interactor" in env:
        os.symlink(f"../{env['interactor']}", f"build/{env['interactor']}")
