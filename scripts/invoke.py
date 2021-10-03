import sys

from .env import env
from .executors import Executor
from .checkers import Checker, display_code


def main(args):
    assert len(args) >= 1, "please specify a program to invoke"

    invoker = Invoker(args[0])
    validator = env.validator

    print("---Fetching Input---")

    inp = sys.stdin.read()

    print("---Fetched Input---\n")

    print("---Input---")

    print(inp, end="")

    if not validator.validate(inp):
        print("---Invalid Case---")

        return -1

    result = invoker.invoke(inp)

    print(f"---{args[0]}-- {result['process_time']}s --")

    print(result["process_result"].stdout, end="")

    print(f"---Reference-- {result['reference_time']}s --")

    print(result["reference_result"].stdout, end="")

    print("---Checker---")

    print(display_code(result["checker_result"]))

    print("-------------")


class Invoker:
    def __init__(self, file):
        self.program = Executor(file)
        self.reference_sol = env.reference_sol
        self.checker = env.checker

    def invoke(self, input):
        process_result, process_time = self.program.run(
            input=input, timeout=env.timeout, return_time=True
        )
        reference_result, reference_time = self.reference_sol.run(
            input=input, timeout=env.timeout, return_time=True
        )

        return dict(
            process_result=process_result,
            reference_result=reference_result,
            checker_result=self.checker.check(
                input, process_result.stdout, reference_result.stdout
            ),
            process_time=process_time,
            reference_time=reference_time,
        )
