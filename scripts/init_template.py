import datetime
import os
import sys
import yaml

SETTER_DIR = os.getenv("SETTER_DIR")

dependencies = yaml.safe_load(open(f"{SETTER_DIR}/cpp_utils/dependencies.yml"))


def main(args):
    if not args:
        print("Please specify a list of files to include.", file=sys.stderr)
        return -1

    for x in args:
        if x not in dependencies:
            print(f"{x} is not a valid file name", file=sys.stderr)
            return -1

    argstring = " ".join(args)
    date = datetime.date.today().isoformat()
    print(f"// Built with `init-template {argstring}` on {date}")
    write_template(args)


def write_template(files):
    deps = get_deps(files)

    for file_name in deps:
        with open(f"{SETTER_DIR}/cpp_utils/{file_name}.cpp") as source_file:
            print(source_file.read(), file=sys.stdout)


def get_deps(files):
    final_deps = []

    def resolve(file_name):
        for sub_dep in dependencies[file_name]:
            resolve(sub_dep)

        if file_name not in final_deps:
            final_deps.append(file_name)

    for f in files:
        resolve(f)

    return final_deps
