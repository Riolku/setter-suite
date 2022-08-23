import sys

from .executors import get_executor


class Validator:
    def __init__(self, file):
        if not file:
            print("Warning! No validator. Proceeding anyway.", file=sys.stderr)

            self.validator = None

        else:
            self.validator = get_executor(file)

    def validate(self, input):
        if self.validator is None:
            return True

        else:
            return self.validator.run(input=input, check_success=False).returncode == 0

    def assertValid(self, input):
        assert self.validate(input), "Invalid input!"
