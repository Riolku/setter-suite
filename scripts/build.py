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

    link_init()
    link_data()
    link_checker()

def link_init():
    os.symlink(f"../init.yml", f"build/init.yml")

def link_data():
    init_type = env["init_type"]

    if init_type == "zip":
        os.symlink("../data/data.zip", "build/data.zip")

    os.symlink(f"../{env['generator']}", f"build/{env['generator']}")

def link_checker():
    if env['checker'].endswith('cpp'):
        os.symlink(f"../{env['checker']}", f"build/{env['checker']}")
