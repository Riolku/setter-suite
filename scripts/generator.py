import multiprocessing, os

from multiprocessing import Pool

from .executors import Executor

import zipfile

def main(args, env):
    gen = Generator(env)

    if os.path.exists("old-data"):
        os.system("rm -r old-data")

    try:
        os.rename("data", "old-data")
    except FileNotFoundError:
        pass

    os.mkdir('data')

    CASE_COUNTS = env['case_counts']

    SUITE_COUNT = len(CASE_COUNTS)

    case_iter = [(suite + 1, case_num + 1) for suite in range(SUITE_COUNT) for case_num in range(CASE_COUNTS[suite])]

    with Pool() as p:
        for suite, case in p.imap_unordered(gen.generate, case_iter):
            print("Generated    %.2d.%.2d" % (suite, case))

    print("---Creating Zip---")

    # I really don't think this can be parallelized effectively
    with zipfile.ZipFile("data/data.zip", "w") as zf:
        for suite, case_num in case_iter:
            print("Adding       %.2d.%.2d" % (suite, case_num))

            with open(f"data/{suite}.{case_num}.in", "rb") as data_f:
                with zf.open(f"{suite}.{case_num}.in", "w") as zip_f:
                    zip_f.write(data_f.read())

                with open(f"data/{suite}.{case_num}.out", "rb") as data_f:
                    with zf.open(f"{suite}.{case_num}.out", "w") as zip_f:
                        zip_f.write(data_f.read())

    print("---Done---")

class Generator:
    def __init__(self, env):
        assert env['generator_type'] in ['single', 'double'], f"Unrecognized generator type '{env['generator_type']}'"

        if env['generator_type'] == 'single':
            self.generate = self.generate_single
            self.generator_executor = Executor(env['generator_file'])

        elif env['generator_type'] == 'double':
            self.generate = self.generate_double
            self.generator_executor = Executor(env['generator_file'])
            self.solution_executor = Executor(env['solutions'][0])

    def generate_single(self, arg):
        suite, case = arg
        with open(f"data/{suite}.{case}.in", "w") as in_f:
            with open(f"data/{suite}.{case}.out", "w") as out_f:
                try:
                    self.generator_executor.run([str(suite), str(case)], stdout = in_f, stderr = out_f)
                except:
                    raise RuntimeError(f"Failed generating case {suite}.{case}")

        return suite, case

    def generate_double(self, arg):
        suite, case = arg
        with open(f"data/{suite}.{case}.in", "w") as in_f:
            try:
                self.generator_executor.run([str(suite), str(case)], stdout = in_f)
            except:
                raise RuntimeError(f"Failed generating case {suite}.{case}")

        with open(f"data/{suite}.{case}.in", "r") as in_f:
            with open(f"data/{suite}.{case}.out", "w") as out_f:
                try:
                    self.solution_executor.run(stdin = in_f, stdout = out_f)
                except:
                    raise RuntimeError(f"Failed generating case {suite}.{case}")

        return suite, case
