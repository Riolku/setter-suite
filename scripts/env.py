import os, json, yaml, multiprocessing

from cached_property import cached_property

from .executors import get_executor
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
            # Dashes stop processing as arguments
            if args[i] == "-":
                i += 1
                newargs.extend(args[i:])
                break

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
        reference_file = self.env.get("reference_sol")
        if reference_file is None:
            if os.path.exists("sol.cpp"):
                reference_file = "sol.cpp"
            if os.path.exists("sol.rs"):
                reference_file = "sol.rs"

        return get_executor(reference_file)

    @cached_property
    def checker_name(self):
        return self.env.get("checker", "standard")

    @cached_property
    def checker_args(self):
        return self.env.get("checker_args", {})

    @cached_property
    def checker(self):
        return Checker.get(self.checker_name, self.checker_args)

    @cached_property
    def validator(self):
        return Validator(self.env.get("validator"))

    @property
    def init_type(self):
        return self.env.get("init_type", "generator")

    @property
    def generator(self):
        return self.env.get("generator", "gen.cpp")

    @property
    def timeout(self):
        return int(self.env.get("timelimit", "5"))

    @property
    def case_counts(self):
        if "case_counts" not in self.env:
            gen = get_executor(self.generator)
            return list(map(int, gen.run(["cc"]).stdout.split()))
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
