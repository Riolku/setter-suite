import subprocess

from .env import env
from .invoke import Invoker
from .checkers import display_code, codes

def main(args):
    assert len(args) >= 1, "Must pass the solution to judge"

    judge = Judge(
        args[0]
    )

    judge.judge()

class Judge:
    def __init__(self, file):
        self.invoker = Invoker(file)

    def judge(self):
        CASE_COUNTS = env.case_counts

        SUITE_COUNT = len(CASE_COUNTS)

        case_iter = [(suite + 1, case_num + 1) for suite in range(SUITE_COUNT) for case_num in range(CASE_COUNTS[suite])]

        print("---Judging---")

        with env.pool() as p:
            for suite, case, result, time in p.imap(self.judge_one, case_iter):
                if time:
                    print("%.2d.%.2d : %s (%s)" % (suite, case, display_code(result), time))
                else:
                    print("%.2d.%.2d : %s" % (suite, case, display_code(result)))

        print("---Done---")

    def judge_one(self, arg):
        suite, case = arg

        with open(f"data/{suite}.{case}.in") as in_f:
            try:
                result = self.invoker.invoke(in_f.read())

                return suite, case, result['checker_result'], result['process_time']

            except RuntimeError:
                return suite, case, codes['RTE'], ''

            except subprocess.TimeoutExpired:
                return suite, case, codes['TLE'], ''
