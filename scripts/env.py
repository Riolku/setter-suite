import json, yaml, multiprocessing

from cached_property import cached_property

from .executors import Executor
from .checkers import Checker
from .validator_base import Validator


class Env:
    def __init__(self):
        self.env = {}

    def parse_from_config(self):
        try:
            with open("config.json", "r") as f:
                self.env.update(json.load(f))

        except FileNotFoundError:
            pass

        try:
            with open("config.yml", "r") as f:
                self.env.update(yaml.safe_load(f))

        except FileNotFoundError:
            pass

    def setup_env(self, args):
        i = 0

        self.parse_from_config()

        newargs = []

        while i < len(args):
            if args[i].startswith("-"):
                assert i + 1 < len(args), f"option {args[i]} requires an argument"

                if args[i][1] == "-":
                    self.env[args[i][2:]] = args[i + 1]
                else:
                    self.env[args[i][1]] = args[i + 1]

                i += 2
            else:
                newargs.append(args[i])

                i += 1

        return newargs

    @cached_property
    def reference_sol(self):
        return Executor(self.env["solutions"][0])

    @cached_property
    def checker(self):
        return Checker.get(self.env["checker"])

    @cached_property
    def validator(self):
        return Validator(self.env.get("validator"))

    @property
    def timeout(self):
        if "timelimit" in self.env:
            return int(self.env["timelimit"])

        else:
            return 5

    @property
    def case_counts(self):
        return self.env["case_counts"]

    @property
    def python_exec(self):
        return self.env.get("python_exec", "python3")

    @property
    def case_iter(self):
        cc = self.case_counts

        return [
            (suite + 1, case_num + 1)
            for suite in range(len(cc))
            for case_num in range(cc[suite])
        ]

    def pool(self):
        if "workers" in self.env:
            workers = int(self.env["workers"])

        else:
            workers = None

        return multiprocessing.Pool(workers)

    def __getitem__(self, key):
        return self.env[key]

    def __contains__(self, key):
        return key in self.env

    def get(self, key, backup=None):
        return self.env.get(key, backup)


env = Env()
