import subprocess, os, time
from subprocess import PIPE
from sys import exit

from ctypes import cdll
import signal

CPP_EXT = ".cpp"
PY_EXT = ".py"
RUST_EXT = ".rs"


def get_executor(file, *, debug=False, force_compile=False):
    if file.endswith(CPP_EXT):
        return CppExecutor(file, debug=debug, force_compile=force_compile)
    elif file.endswith(PY_EXT):
        return PyExecutor(file, debug=debug, force_compile=force_compile)
    else:
        assert file.endswith(
            RUST_EXT
        ), f"Refusing to run '{file}' with unknown extension"
        return RustExecutor(file, debug=debug, force_compile=force_compile)


class Executor:
    def __init__(self, file, *, debug=False, force_compile=False):
        self.file = file
        self.compile(debug=debug, force_compile=force_compile)
        self.exec = self.get_exec()

    def compile(self, *, debug, force_compile) -> None:
        pass

    def get_exec(self):
        raise NotImplementedError

    def run(self, args=[], **kwargs):
        kwargs.setdefault("text", True)
        kwargs.setdefault("stdout", PIPE)
        kwargs.setdefault("stderr", PIPE)
        kwargs.setdefault("timeout", 10)

        check_success = kwargs.pop("check_success", True)
        return_time = kwargs.pop("return_time", False)

        kwargs.setdefault("preexec_fn", do_prctl_deathsig)
        kwargs.setdefault("env", self.get_default_env())

        start = time.time()

        ret = subprocess.run(self.exec + args, **kwargs)

        end = time.time()

        if check_success != False and ret.returncode != 0:
            raise RuntimeError(
                f"{self.file} called with {args} failed with {ret.returncode}\n{ret.stderr}"
            )

        if return_time:
            return ret, "%.3f" % (end - start)

        else:
            return ret

    def get_default_env(self):
        return {}


class CompiledExecutor(Executor):
    ext = None

    def compile(self, *, debug: bool, force_compile: bool) -> None:
        self.compiled_file = self.file[: -len(self.ext)]
        try:
            if force_compile or more_recent(self.file, self.compiled_file):
                self.force_compile(debug=debug)

        except FileNotFoundError:
            self.force_compile(debug=debug)

    def force_compile(self, *, debug: bool) -> None:
        assert subprocess.run(self.get_compiler_cmd(debug=debug)).returncode == 0

    def get_compiler_cmd(self):
        raise NotImplementedError

    def get_exec(self):
        return ["./" + self.compiled_file]


class CppExecutor(CompiledExecutor):
    ext = CPP_EXT

    def get_compiler_cmd(self, *, debug: bool):
        if debug:
            return [
                "g++",
                "-Wall",
                "-std=c++17",
                "-g",
                "-o",
                self.compiled_file,
                self.file,
            ]

        return [
            "g++",
            "-O2",
            "-Wall",
            "-std=c++17",
            "-g",
            "-o",
            self.compiled_file,
            self.file,
        ]


class PyExecutor(Executor):
    ext = PY_EXT

    def get_exec(self):
        from .env import env

        return [env.python_exec, self.file]


class RustExecutor(CompiledExecutor):
    ext = RUST_EXT

    def get_compiler_cmd(self, *, debug: bool):
        if debug:
            return ["rustc", "-g", self.file]
        return ["rustc", "-g", "-O", self.file]

    def get_default_env(self):
        return dict(RUST_BACKTRACE=1)


def do_prctl_deathsig():
    PR_SET_PDEATHSIG = 1
    cdll["libc.so.6"].prctl(PR_SET_PDEATHSIG, signal.SIGKILL)


def more_recent(A: str, B: str) -> bool:
    A_info = os.stat(A)
    B_info = os.stat(B)

    return A_info.st_mtime > B_info.st_mtime
