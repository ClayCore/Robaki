from utils.types import Dependency

from builders.common import Error, Result
from builders import *

from classopt import classopt, config
import colorama

import importlib
from pathlib import Path
import sys


@classopt(default_long=True)
class Opt:
    # Describes launch parameters

    action: str       # Action to perform
    root_path: str    # Path to project root
    deps: list[str]   # Dependencies


# Acquire a dictionary, with paths pointing to each dependency
def get_all_deps(opts: Opt) -> dict:
    root_tmp = Path(opts.root_path)

    root = root_tmp.resolve()
    deps = {}
    for dep in opts.deps:
        if dep == 'bgfx':
            # add bimg and bgfx
            deps['bimg'] = Dependency.create('bimg', root)
            deps['bx'] = Dependency.create('bx', root)
            deps[dep] = Dependency.create(dep, root)
        else:
            deps[dep] = Dependency.create(dep, root)

    return deps


def add_builder(name: str, opt: Opt, deps: dict, root_path: Path):
    if name in opt.deps:
        module = importlib.import_module(f'builders.{name}')
        builder = getattr(module, f'{name.upper()}Builder')(root_path, deps)
        if opt.action == 'build':
            result = builder.prepare()
            if result.error != Error.SUCCESS:
                print(result.result)
                raise RuntimeError(
                    f'[{name.upper()}]: failed to prepare build')

            result = builder.build()
            if result.error != Error.SUCCESS:
                print(result.result)
                raise RuntimeError(
                    f'[{name.upper()}]: failed to execute build')
        elif opt.action == 'clean':
            result = builder.clean()
            if result.error != Error.SUCCESS:
                print(result.result)
                raise RuntimeError(
                    f'[{name.upper()}]: failed to execute clean')


def main():
    colorama.init()

    # ==============================================================================================
    # Parsing launch parameters
    # ==============================================================================================
    opt = Opt.from_args()
    deps = get_all_deps(opt)

    # ==============================================================================================
    # Create all the builders
    # ==============================================================================================
    root_path: Path = Path(opt.root_path).resolve()
    for dep in deps.keys():
        try:
            add_builder(dep, opt, deps, root_path)
        except RuntimeError as re:
            print(
                f'{colorama.Fore.RED}RuntimeError caught: {colorama.Style.BRIGHT}{colorama.Fore.BLUE}{re}{colorama.Style.RESET_ALL}\n', file=sys.stderr)
            return 1


if __name__ == "__main__":
    main()
