from .executors import Executor

class Validator:
    def __init__(self, file):
        self.validator = Executor(file)

    def validate(self, input):
        return self.validator.run(input = input, check_success = False).returncode == 0

    def assertValid(self, input):
        assert self.validate(input), "Invalid input!"
