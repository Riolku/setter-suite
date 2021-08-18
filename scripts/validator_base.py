from .executors import Executor

class Validator:
    def __init__(self, file):
        if not file:
            print("Warning! No validator. Proceeding anyway.")

            self.validator = None

        else:
            self.validator = Executor(file)

    def validate(self, input):
        if self.validator is None:
            return True

        else:
            return self.validator.run(input = input, check_success = False).returncode == 0

    def assertValid(self, input):
        assert self.validate(input), "Invalid input!"
