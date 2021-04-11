import json

def parse_config():
    try:
        with open("config.json", "r") as f:
            return json.load(f)

    except FileNotFoundError:
        return {}

def parse_env(args):
    i = 0

    env = parse_config()

    newargs = []

    while i < len(args):
        if args[i].startswith("-"):
            if args[i][1] == '-':
                env[args[i][2:]] = args[i + 1]
            else:
                env[args[i][1]] = args[i + 1]

            i += 2
        else:
            newargs.append(args[i])

            i += 1

    return newargs, env
