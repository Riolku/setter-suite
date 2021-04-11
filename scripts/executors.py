import subprocess
from subprocess import PIPE
from sys import exit

CPP_EXT = ".cpp"
PY_EXT = ".py"

class Executor:
    def __init__(self, file, use_cpy = True):
        self.file = file

        if file.endswith(CPP_EXT):
            file_root = file[:-len(CPP_EXT)]

            assert subprocess.run(["g++", "-O2", "-Wall", "-std=c++17", "-o", file_root, file]).returncode == 0

            self.exec = ["./" + file_root]

        else:
            assert file.endswith(PY_EXT)

            self.exec = ["pypy3", file]

            if use_cpy:
                self.exec[0] = "python3"

    def run(self, args = [], **kwargs):
        kwargs.setdefault("text", True)
        kwargs.setdefault("stdout", PIPE)
        kwargs.setdefault("stderr", PIPE)

        ret = subprocess.run(self.exec + args, **kwargs)

        if kwargs.get('check_success', True) != False and ret.returncode != 0:
            raise RuntimeError(f"{self.file} failed with {ret.returncode}\n{ret.stderr}")

        return ret
