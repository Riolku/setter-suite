import multiprocessing

from multiprocessing import Pool

from .executors import Executor

DEFAULT_BLOCKSIZE = 100

def main(args, env):
    if len(args) < 3:
        print("generator and two solutions required.")
        return -1

    blocksize = int(env.get("b", env.get("blocksize", DEFAULT_BLOCKSIZE)))

    generator = Executor(args[0])
    sol_1 = Executor(args[1])
    sol_2 = Executor(args[2])

    CC = CounterCaser(generator, sol_1, sol_2)

    CC.run(blocksize)

def inf_iter():
    c = 1
    while 1:
        yield c
        c += 1

class CounterCaser:
    def __init__(self, generator, sol_1, sol_2):
        self.generator = generator
        self.sol_1 = sol_1
        self.sol_2 = sol_2

    def run(self, blocksize):
        counter = 0

        with Pool() as p:
            for feedback in p.imap_unordered(self.run_one, inf_iter()):
                if feedback is not None:
                    print(feedback, end = "")

                    break

                counter += 1

                if counter % blocksize == 0:
                    print(f"Tested {counter} cases.")


    def run_one(self, run_num):
        case = self.generator.run()

        out1 = self.sol_1.run(input = case.stdout)
        out2 = self.sol_2.run(input = case.stdout)

        if out1.stdout != out2.stdout:
            return f"===CASE===\n{case.stdout}==={self.sol_1.file}===\n{out1.stdout}==={self.sol_2.file}===\n{out2.stdout}=======\n"

        return None
