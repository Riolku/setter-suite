import subprocess, os, time
from subprocess import PIPE
from sys import exit

CPP_EXT = ".cpp"
PY_EXT = ".py"


class Executor:
    def __init__(self, file, use_cpy=True):
        self.file = file

        if file.endswith(CPP_EXT):
            file_root = file[: -len(CPP_EXT)]

            self.compile(file, file_root)

            self.exec = ["./" + file_root]

        else:
            assert file.endswith(
                PY_EXT
            ), f"Refusing to run '{file}' with unknown extension"

            self.exec = ["pypy3", file]

            if use_cpy:
                self.exec[0] = "python3"

    def compile(self, file, file_root):
        try:
            compiled_stat_info = os.stat(file_root)
            source_stat_info = os.stat(file)

            # compile only if the source file is more recent than the executable
            if source_stat_info.st_mtime >= compiled_stat_info.st_mtime:
                self.force_compile(file, file_root)

        except FileNotFoundError:
            self.force_compile(file, file_root)

    def force_compile(self, file, file_root):
        assert (
            subprocess.run(
                ["g++", "-O2", "-Wall", "-g", "-std=c++20", "-o", file_root, file]
            ).returncode
            == 0
        )

    def run(self, args=[], **kwargs):
        kwargs.setdefault("text", True)
        kwargs.setdefault("stdout", PIPE)
        kwargs.setdefault("stderr", PIPE)
        kwargs.setdefault("timeout", 10)

        check_success = kwargs.pop("check_success", True)
        return_time = kwargs.pop("return_time", False)

        kwargs.setdefault("preexec_fn", do_prctl_deathsig)

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


def do_prctl_deathsig():
    from ctypes import cdll
    import signal

    PR_SET_PDEATHSIG = 1

    cdll["libc.so.6"].prctl(PR_SET_PDEATHSIG, signal.SIGKILL)
