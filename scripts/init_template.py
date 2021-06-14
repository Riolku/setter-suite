import json, os, sys

SETTER_DIR = os.getenv("SETTER_DIR")

dependencies = json.load(open(f"{SETTER_DIR}/cpp_utils/dependencies.json"))

templates = dependencies['templates']

files = dependencies['files']

def main(args, env):
    if not args:
        print("Please specify a template.")
        return -1

    if args[0] not in templates:
        print(f"Invalid template '{args[0]}'")
        return -1

    template_conf = templates[args[0]]

    write_template(template_conf)

def write_template(conf):
    deps = get_deps(conf['deps'])

    with open(conf['name'], "w") as out_file:
        for file_name in deps:
            with open(f"{SETTER_DIR}/cpp_utils/{file_name}") as source_file:
                print(source_file.read(), file = out_file)

def get_deps(deps):
    final_deps = ["sys_deps.hpp"]

    def resolve(file_name):
        for sub_dep in files[file_name]:
            resolve(sub_dep)

        if file_name not in final_deps:
            final_deps.append(file_name)

    for dep in deps:
        resolve(dep)

    return final_deps
