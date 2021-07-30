from .env import env

def main(args):
    validator = DataValidator()

    with env.pool() as p:
        for suite, case, result in p.imap(validator.validate_one, env.case_iter):
            if result is not True:
                print(f"%.2d.%.2d is invalid!" % (suite, case))

                break

            else:
                print(f"Validated   %.2d.%.2d" % (suite, case))

class DataValidator:
    def __init__(self):
        self.validator = env.validator

    def validate_one(self, arg):
        suite, case = arg

        with open(f"data/{suite}.{case}.in", "r") as f:
            return suite, case, self.validator.validate(f.read())
