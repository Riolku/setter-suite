import multiprocessing, os

from .executors import Executor

AC = 0
WA = 1
PE = 2
IE = 7

class Checker:
    def get(id):
        if id.endswith("cpp"):
            return CustomChecker(id)

        else:
            return BuiltinChecker.get(id)

class BuiltinChecker:
    def __init__(self):
        return NotImplementedError

    def get(id):
        if id == "identical":
            return IdenticalChecker()

        raise NotImplementedError(f"no such checker '{id}'")

class IdenticalChecker:
    def __init__(self):
        pass

    def check(self, inp, out, expected_out):
        if out == expected_out:
            return AC;

        return WA

class CustomChecker:
    def __init__(self, filename):
        self.executor = Executor(filename)

    def check(self, inp, out, expected_out):
        pid = multiprocessing.current_process().pid

        with open(f"/tmp/{pid}.in", "w") as f:
            f.write(inp)

        with open(f"/tmp/{pid}.out", "w") as f:
            f.write(out)

        with open(f"/tmp/{pid}.eout", "w") as f:
            f.write(expected_out)

        ret = self.executor.run([f"/tmp/{pid}.in", f"/tmp/{pid}.out", f"/tmp/{pid}.eout"], check_success = False)

        os.remove(f"/tmp/{pid}.in")
        os.remove(f"/tmp/{pid}.out")
        os.remove(f"/tmp/{pid}.eout")

        return ret.returncode
