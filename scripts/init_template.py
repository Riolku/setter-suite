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

    def isdummy(file_name):
        return "DUMMY" in dependencies[file_name]

    def resolve(file_name):
        for sub_dep in dependencies[file_name]:
            if sub_dep != "DUMMY":
                resolve(sub_dep)
                if not isdummy(sub_dep) and sub_dep not in final_deps:
                    final_deps.append(sub_dep)

    for f in files:
        resolve(f)

    # Ensure order of the files given on the command line
    # Helpful when we want to have more control on the output ordering
    for f in files:
        if not isdummy(f):
            if f in final_deps:
                final_deps.remove(f)
            final_deps.append(f)

    return final_deps
