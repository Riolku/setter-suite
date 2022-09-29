import datetime
import os
import sys
import toml

SETTER_DIR = os.getenv("SETTER_DIR")

dependencies = toml.load(open(f"{SETTER_DIR}/rust_utils/Cargo.toml"))["features"]


def main(args):
    if not args:
        print("Please specify a list of files to include.", file=sys.stderr)
        return -1

    result = validate_files(args)
    if result != 0:
        return result

    write_template(args, output_file=sys.stdout)


def validate_files(files):
    for x in files:
        if x not in dependencies:
            print(f"{x} is not a valid file name", file=sys.stderr)
            return -1

    return 0


def write_template(files, *, output_file):
    deps = get_deps(files)

    filestring = " ".join(files)
    date = datetime.date.today().isoformat()
    print(f"// Built with dependencies `{filestring}` on {date}", file=output_file)
    print("pub mod dmoj_rust_utils {", file=output_file)
    for file_name in deps:
        try:
            source_file = open(f"{SETTER_DIR}/rust_utils/src/{file_name}.rs")
        except FileNotFoundError:
            source_file = open(f"{SETTER_DIR}/rust_utils/src/{file_name}/mod.rs")

        with source_file:
            print("    pub mod %s {" % file_name, file=output_file)
            print_source_indent(
                source_file.read().replace("#[cfg(test)]\nmod tests;\n", "").strip(),
                spaces=8,
                output_file=output_file,
            )
            print("    }", file=output_file)

    print("}", file=output_file)
    print("// End of template", file=output_file)


def get_deps(files):
    final_deps = []

    def resolve(file_name):
        if file_name not in final_deps:
            final_deps.append(file_name)
        for sub_dep in dependencies[file_name]:
            resolve(sub_dep)

    for f in files:
        resolve(f)

    return final_deps


def print_source_indent(contents: str, *, spaces: int, output_file) -> str:
    for line in contents.splitlines():
        if line.strip() == "":
            print(file=output_file)
        else:
            print(" " * spaces + line, file=output_file)
