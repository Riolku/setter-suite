from .invoke import Invoker
from .env import env
from .executors import get_executor
from .checkers import codes, display_code

DEFAULT_BLOCKSIZE = 100


def main(args):
    assert len(args) >= 1, "solution required"

    blocksize = int(env.get("b", env.get("blocksize", DEFAULT_BLOCKSIZE)))

    generator = get_executor(env.get("random_gen", "random_gen.py"))

    invoker = Invoker(args[0])

    CC = CounterCaser(generator, invoker)

    CC.run(blocksize)


def inf_iter():
    c = 1
    while 1:
        yield c
        c += 1


class CounterCaser:
    def __init__(self, generator, invoker):
        self.generator = generator
        self.invoker = invoker
        self.validator = env.validator

    def run(self, blocksize):
        counter = 0

        with env.pool() as p:
            for feedback in p.imap_unordered(self.run_one, inf_iter()):
                if feedback is not None:
                    print(feedback, end="")

                    break

                counter += 1

                if counter % blocksize == 0:
                    print(f"Tested {counter} cases.")

    def run_one(self, run_num):
        generator_res = self.generator.run(timeout=10)

        case = generator_res.stdout

        if not self.validator.validate(case):
            return (
                f"===CASE===\n{case}===CASE INVALID===\n"
                + "===Gen Stderr===\n"
                + f"{generator_res.stderr}"
            )

        try:
            result = self.invoker.invoke(case)

            checker_res = result["checker_result"]

            if checker_res != codes["AC"]:
                inp_rep = repr(case).replace("'", '"')
                out_rep = repr(result["reference_result"].stdout).replace("'", '"')

                return (
                    f"===CASE===\n{case}==={self.invoker.program.file}===\n{result['process_result'].stdout}"
                    + f"===Reference===\n{result['reference_result'].stdout}===CHECKER===\n{display_code(checker_res)}\n"
                    + "=========\n"
                    + "===Gen Stderr===\n"
                    + f"{generator_res.stderr}"
                    + "===LiteralTest Config===\n"
                    + f"make_shared<LiteralTest>({inp_rep}, {out_rep}),\n"
                    + f"LiteralTest::new({inp_rep}, {out_rep}),\n"
                    + "=========\n"
                )

            return None
        except Exception as e:
            raise Exception(f"==input===\n{case}=======\n")
