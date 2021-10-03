import os

from .env import env
from .executors import Executor

import zipfile


def main(args):
    gen = Generator()

    if os.path.exists("old-data"):
        os.system("rm -r old-data")

    try:
        os.rename("data", "old-data")
    except FileNotFoundError:
        pass

    os.mkdir("data")

    with env.pool() as p:
        for suite, case in p.imap(gen.generate, env.case_iter):
            print("Generated    %.2d.%.2d" % (suite, case))

    print("---Creating Zip---")

    # I really don't think this can be parallelized effectively
    # Nor is it slow
    with zipfile.ZipFile("data/data.zip", "w") as zf:
        for suite, case_num in env.case_iter:
            print("Adding       %.2d.%.2d" % (suite, case_num))

            with open(f"data/{suite}.{case_num}.in", "rb") as data_f:
                with zf.open(f"{suite}.{case_num}.in", "w") as zip_f:
                    zip_f.write(data_f.read())

                with open(f"data/{suite}.{case_num}.out", "rb") as data_f:
                    with zf.open(f"{suite}.{case_num}.out", "w") as zip_f:
                        zip_f.write(data_f.read())

    print("---Done---")


class Generator:
    def __init__(self):
        self.generator_executor = Executor(env["generator"])

        if env.get("generator_type") == "double":
            self.generate = self.generate_double
            self.solution_executor = env.reference_sol

        else:
            self.generate = self.generate_single

    def generate_single(self, arg):
        suite, case = arg
        with open(f"data/{suite}.{case}.in", "w") as in_f:
            with open(f"data/{suite}.{case}.out", "w") as out_f:
                try:
                    self.generator_executor.run(
                        [str(suite), str(case)], stdout=in_f, stderr=out_f
                    )
                except:
                    raise RuntimeError(f"Failed generating case {suite}.{case}")

        return suite, case

    def generate_double(self, arg):
        suite, case = arg
        with open(f"data/{suite}.{case}.in", "w") as in_f:
            try:
                self.generator_executor.run([str(suite), str(case)], stdout=in_f)
            except:
                raise RuntimeError(f"Failed generating case {suite}.{case}")

        with open(f"data/{suite}.{case}.in", "r") as in_f:
            with open(f"data/{suite}.{case}.out", "w") as out_f:
                try:
                    self.solution_executor.run(stdin=in_f, stdout=out_f)
                except:
                    raise RuntimeError(f"Failed generating case {suite}.{case}")

        return suite, case
