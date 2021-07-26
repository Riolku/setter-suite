import multiprocessing, os, subprocess

from multiprocessing import Pool

from .executors import Executor
from .checkers import Checker, codes, rcodes

import zipfile

def main(args, env):
    assert len(args) >= 1, "Must pass the solution to judge"

    CASE_COUNTS = env['case_counts']

    SUITE_COUNT = len(CASE_COUNTS)

    case_iter = [(suite + 1, case_num + 1) for suite in range(SUITE_COUNT) for case_num in range(CASE_COUNTS[suite])]

    checker = Checker.get(env['checker'])

    judge = Judge(
        Executor(args[0]),
        checker
    )

    workers = env.get("workers")

    if workers: workers = int(workers)

    print("---Judging---")

    with Pool(workers) as p:
        for suite, case, result in p.imap(judge.judge, case_iter):
            print("%.2d.%.2d : %s" % (suite, case, rcodes.get(result, result)))

    print("---Done---")

class Judge:
    def __init__(self, sol, checker):
        self.sol = sol
        self.checker = checker

    def judge(self, arg):
        suite, case = arg

        with open(f"data/{suite}.{case}.in") as in_f:
            try:
                proc_result = self.sol.run(stdin = in_f, timeout = 10)

            except RuntimeError:
                return suite, case, codes['RTE']

            except subprocess.TimeoutExpired:
                return suite, case, codes['TLE']

        with open(f"data/{suite}.{case}.in") as in_f:
            with open(f"data/{suite}.{case}.out") as out_f:
                return suite, case, self.checker.check(in_f.read(), proc_result.stdout, out_f.read())
