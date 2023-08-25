from pathlib import Path
import shutil

from . import common as cm
import subprocess as sp
import os
import shlex


class SPDLOGBuilder(cm.Builder):
    def __init__(self, root_path: Path, deps: dict):
        super().__init__(root_path, deps, 'spdlog')

    def prepare(self) -> cm.Result:
        # ==============================================================================================
        # spdlog is a header only library, no need to build anything
        # ==============================================================================================

        # ==============================================================================================
        # Create include directory and copy headers
        # ==============================================================================================
        if not self.target_include_dir.exists():
            self.target_include_dir.mkdir(parents=True)

        cm.Builder.copytree(self.include_dir, self.target_include_dir)
        if not self.target_include_dir.exists():
            msg = f'[SPDLOG]: failed to transact copy to \'{str(self.target_include_dir)}\''
            return cm.Result(cm.Error.FILE_COPY_FAILED, msg)

        return super().prepare()

    def build(self) -> cm.Result:
        return super().build()

    def clean(self) -> cm.Result:
        return super().clean()
