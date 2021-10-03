import yaml

from .env import env

case_regex = "(?P<batch>\d+)\.(?P<case>\d+).in"


def main(main):
    ret = {}

    add_base(ret)
    add_checker(ret)
    add_cases(ret)

    with open("init.yml", "w") as f:
        f.write(yaml.dump(ret))

    return 0


def add_base(ret):
    ret["output_limit_length"] = env.get("output_limit_length", 10 ** 6)
    ret["output_prefix_length"] = env.get("output_prefix_length", 0)


def add_checker(ret):
    checker = env["checker"]

    if checker.endswith("cpp"):
        ret["checker"] = dict(
            name="bridged",
            args=dict(
                files=checker,
                lang=env.get("checker_lang", "CPP17"),
                feedback=env.get("checker_feedback", True),
                type=env.get("checker_type", "coci"),
                cached=env.get("cached", True),
            ),
        )

    else:
        assert (
            checker.find(".") == -1
        ), "Checker must be a builtin checker if not bridged!"

        ret["checker"] = checker


def add_cases(
    ret,
):
    init_type = env["init_type"]

    assert len(env["case_counts"]) == len(
        env["case_points"]
    ), "must have the same number of cases in case_counts and case_points"

    if init_type == "generator":
        assert (
            env.get("generator_type", "single") == "single"
        ), "Cannot use 'double' generator config with 'generator' init_type"

        ret["generator"] = env["generator"]

        test_cases = []

        for suite_num in range(len(env["case_counts"])):
            test_cases.append(dict(batched=[]))

            for case_num in range(env["case_counts"][suite_num]):

                test_cases[-1]["batched"].append(
                    dict(generator_args=[suite_num + 1, case_num + 1])
                )

            test_cases[-1]["points"] = env["case_points"][suite_num]

        ret["test_cases"] = test_cases

    else:
        assert init_type == "zip", "Unknown init_type"

        ret["archive"] = "data.zip"

        ret["test_cases"] = dict(
            input_format=case_regex, case_points=env["case_points"]
        )
