import multiprocessing, os

from termcolor import colored
from typing import Tuple

from .executors import get_executor

codes = dict(AC=0, WA=1, PE=2, IE=3, PARTIAL=7, TLE=1000, RTE=2000)
colors = dict(
    AC="green",
    WA="red",
    PE="cyan",
    IE="magenta",
    PARTIAL="green",
    TLE="blue",
    RTE="yellow",
)

rcodes = {b: a for a, b in codes.items()}


def display_code(code):
    name = rcodes.get(code, "IE")
    return colored(name, colors.get(name))


class Checker:
    def get(id):
        if id.endswith("cpp") or id.endswith("py"):
            return CustomChecker(id)

        else:
            return BuiltinChecker.get(id)


class BuiltinChecker:
    def __init__(self):
        raise NotImplementedError

    def get(id):
        if id == "identical":
            return IdenticalChecker()

        if id == "standard":
            return StandardChecker()

        raise NotImplementedError(f"no such checker '{id}'")


class IdenticalChecker(BuiltinChecker):
    def __init__(self):
        pass

    def check(self, inp, out, expected_out):
        if out == expected_out:
            return codes["AC"]

        return codes["WA"]


def isline(c: str) -> bool:
    return c in "\n\r"


def iswhite(c: str) -> bool:
    return c in " \t\v\f\n\r"


class StandardChecker(BuiltinChecker):
    def __init__(self):
        pass

    def _skip_spaces(self, s: str, pos: int) -> Tuple[int, int]:
        saw_line = saw_space = 0
        while pos < len(s):
            saw_line |= isline(s[pos])
            if not iswhite(s[pos]):
                break
            pos += 1
            saw_space = 1

        return (2 if saw_line else saw_space, pos)

    def _check(self, inp: str, process: str, judge: str) -> bool:
        j = 0
        p = 0
        while j < len(judge) and iswhite(judge[j]):
            j += 1

        while p < len(process) and iswhite(process[p]):
            p += 1

        while True:
            js, j = self._skip_spaces(judge, j)
            ps, p = self._skip_spaces(process, p)

            if j == len(judge) or p == len(process):
                return j == len(judge) and p == len(process)

            if js != ps:
                return False

            while j < len(judge) and not iswhite(judge[j]):
                if p >= len(process):
                    return False

                if judge[j] != process[p]:
                    return 0

                p += 1
                j += 1

    def check(self, inp: str, process: str, judge: str) -> int:
        if self._check(inp, process, judge):
            return codes["AC"]
        else:
            return codes["WA"]


class CustomChecker:
    def __init__(self, filename):
        self.executor = get_executor(filename)

    def check(self, inp, out, expected_out):
        pid = multiprocessing.current_process().pid

        with open(f"/tmp/{pid}.in", "w") as f:
            f.write(inp)

        with open(f"/tmp/{pid}.out", "w") as f:
            f.write(out)

        with open(f"/tmp/{pid}.eout", "w") as f:
            f.write(expected_out)

        ret = self.executor.run(
            [f"/tmp/{pid}.in", f"/tmp/{pid}.out", f"/tmp/{pid}.eout"],
            check_success=False,
        )

        os.remove(f"/tmp/{pid}.in")
        os.remove(f"/tmp/{pid}.out")
        os.remove(f"/tmp/{pid}.eout")

        return ret.returncode
