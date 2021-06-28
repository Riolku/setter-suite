import json, yaml

def parse_config():
    conf = {}

    try:
        with open("config.json", "r") as f:
            conf.update(json.load(f))

    except FileNotFoundError:
        pass

    try:
        with open("config.yml", "r") as f:
            conf.update(yaml.safe_load(f))

    except FileNotFoundError:
        pass

    return conf

def parse_env(args):
    i = 0

    env = parse_config()

    newargs = []

    while i < len(args):
        if args[i].startswith("-"):
            assert i + 1 < len(args), f"option {args[i]} requires an argument"

            if args[i][1] == '-':
                env[args[i][2:]] = args[i + 1]
            else:
                env[args[i][1]] = args[i + 1]

            i += 2
        else:
            newargs.append(args[i])

            i += 1

    return newargs, env
