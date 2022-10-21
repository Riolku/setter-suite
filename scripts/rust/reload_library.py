from sys import stderr

from .init_template import validate_files, write_template


def main(args):
    assert len(args) >= 1, "You must at least pass the file to update"

    rest = ""

    with open(args[0]) as source_file:
        source_lines_iter = source_file.readlines().__iter__()
        for line in source_lines_iter:
            if line.strip() == "// End of template":
                break

        else:
            print("No template footer found in source file, aborting", file=stderr)
            return -1

        for line in source_lines_iter:
            rest += line

    result = validate_files(args[1:])
    if result != 0:
        return result

    with open(args[0], "w") as source_file:
        ret = write_template(args[1:], output_file=source_file)
        if ret:
            return ret
        source_file.write(rest)
