from pathlib import Path
from dataclasses import dataclass
from enum import Enum, auto
from typing import Any
import os
import shutil


class Error(Enum):
    SUCCESS = 1
    IO_ERROR = auto()
    BUILD_TOOL_ERROR = auto()
    HEADER_NOT_FOUND = auto()
    LINKED_DEP_NOT_FOUND = auto()
    FILE_MISSING = auto()
    FILE_COPY_FAILED = auto()
    FILE_EXISTS_WARNING = auto()


@dataclass
class Result(object):
    error: Error = Error.SUCCESS
    result: Any = None


class Builder():
    @staticmethod
    def copytree(src, dst, ignore=None):
        if os.path.isdir(src):
            if not os.path.isdir(dst):
                os.makedirs(dst)
            files = os.listdir(src)
            if ignore is not None:
                ignored = ignore(src, files)
            else:
                ignored = set()
            for f in files:
                if f not in ignored:
                    Builder.copytree(os.path.join(src, f),
                                     os.path.join(dst, f), ignore)
        else:
            shutil.copyfile(src, dst)

    def __init__(self, root_path: Path, deps: dict, name: str):
        self.root_path: Path = root_path
        self.deps: dict = deps
        self.name: str = name

        self.build_dir: Path = self.root_path / 'vendor' / self.name / 'build'
        self.include_dir: Path = self.root_path / 'vendor' / self.name / 'include'

        self.target_build_dir: Path = self.root_path / 'deps' / self.name / 'build'
        self.target_include_dir: Path = self.root_path / 'deps' / self.name / 'include'

    def prepare(self) -> Result:
        return Result(Error.SUCCESS, None)

    def build(self) -> Result:
        return Result(Error.SUCCESS, None)

    def clean(self) -> Result:
        return Result(Error.SUCCESS, None)
