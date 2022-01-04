import os


def newer(file, depends):
    return os.stat(file).st_mtime < os.stat(depends).st_mtime


def main(args):
    pass
