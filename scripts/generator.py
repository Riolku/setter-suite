import multiprocessing, os

from multiprocessing import Pool

from .executors import Executor

def main(args, env):
    gen = Generator(env)

    try:
        os.mkdir('data')
    except FileExistsError:
        pass

    CASE_COUNTS = env['case_counts']

    SUITE_COUNT = len(CASE_COUNTS)

    with Pool() as p:
        p.starmap(
            gen.generate,
            [(suite + 1, case_num + 1) for suite in range(SUITE_COUNT) for case_num in range(CASE_COUNTS[suite])]
        )

class Generator:
    def __init__(self, env):
        assert env['generator_type'] in ['single', 'double'], f"Unrecognized generator type '{env['generator_type']}'"

        if env['generator_type'] == 'single':
            self.generate = self.generate_single
            self.generator_executor = Executor(env['generator_file'])

        elif env['generator_type'] == 'double':
            self.generate = self.generate_double
            self.generator_executor = Executor(env['generator_file'])
            self.solution_executor = Executor(env['solution_file'])

    def generate_single(self, suite, case):
        with open(f"data/{suite}.{case}.in", "w") as in_f:
            with open(f"data/{suite}.{case}.out", "w") as out_f:
                self.generator_executor.run([str(suite), str(case)], stdout = in_f, stderr = out_f)

    def generate_double(self, suite, case):
        with open(f"data/{suite}.{case}.in", "w") as in_f:
            self.generator_executor.run([str(suite), str(case)], stdout = in_f)

        with open(f"data/{suite}.{case}.in", "r") as in_f:
            with open(f"data/{suite}.{case}.out", "w") as out_f:
                self.solution_executor.run(stdin = in_f, stdout = out_f)
