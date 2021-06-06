import os

def main(args, env):

    os.system("rm -r old-build")

    try:
        os.rename("build", "old-build")
    except FileNotFoundError:
        pass

    os.mkdir("build")

    link_init(env)
    link_data(env)
    link_checker(env)

def link_init(env):
    os.symlink(f"../init.yml", f"build/init.yml")

def link_data(env):
    init_type = env["init_type"]

    if init_type == "zip":
        os.symlink("../data/data.zip", "build/data.zip")

    os.symlink(f"../{env['generator_file']}", f"build/{env['generator_file']}")

def link_checker(env):
    if env['checker'].endswith('cpp'):
        os.symlink(f"../{env['checker']}", f"build/{env['checker']}")
