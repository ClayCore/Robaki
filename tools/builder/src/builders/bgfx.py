from pathlib import Path
import shutil

from . import common as cm
import subprocess as sp
import os
import shlex


class BGFXBuilder(cm.Builder):
    def __init__(self, root_path: Path, deps: dict):
        super().__init__(root_path, deps, 'bgfx')

    def prepare(self) -> cm.Result:
        # ==============================================================================================
        # bgfx, bimg, and bx dont utilize a build directory
        # and have to be built together in one project
        # ==============================================================================================
        if not self.target_build_dir.exists():
            self.target_build_dir.mkdir(parents=True)

        # ==============================================================================================
        # Create include directory and copy headers
        # ==============================================================================================
        if not self.target_include_dir.exists():
            self.target_include_dir.mkdir(parents=True)

        cm.Builder.copytree(self.include_dir, self.target_include_dir)
        if not self.target_include_dir.exists():
            msg = f'[BGFX]: failed to transact copy to \'{str(self.target_include_dir)}\''
            return cm.Result(cm.Error.FILE_COPY_FAILED, msg)

        return super().prepare()

    def build(self) -> cm.Result:
        # ==============================================================================================
        # Ensure linked dependencies (bimg, bx) also exist
        # ==============================================================================================
        bimg_exists = self.deps['bimg'].exists()
        bx_exists = self.deps['bx'].exists()

        if (not bimg_exists) or (not bx_exists):
            msg = '[BGFX]: bimg and bx must be present for build'
            return cm.Result(cm.Error.LINKED_DEP_NOT_FOUND, msg)

        # ==============================================================================================
        # Save current cwd
        # ==============================================================================================
        old_cwd: str = os.getcwd()

        # ==============================================================================================
        # Change cwd to build bgfx
        # ==============================================================================================
        cwd: Path = self.deps[self.name].include_dir.parent
        os.chdir(str(cwd))

        # ==============================================================================================
        # Run genie
        # TODO: change platform and genie generation based on OS
        # ==============================================================================================
        cmd = shlex.split('../bx/tools/bin/windows/genie vs2019')
        result: sp.CompletedProcess = sp.run(cmd)
        if result.returncode != 0:
            msg = f'[BGFX]: genie return code: {result.returncode}'
            msg += f'\nstdout: {result.stdout}'
            msg += f'\nstderr: {result.stderr}'
            return cm.Result(cm.Error.BUILD_TOOL_ERROR, msg)

        # ==============================================================================================
        # Use 'msbuild' to build everything
        # FIXME: msbuild not used on any platform besides windows
        # ==============================================================================================
        cmd = shlex.split(
            'msbuild .build/projects/vs2019/bgfx.sln /clp:ErrorsOnly /p:Configuration="Release" /p:Platform="x64"')
        result: sp.CompletedProcess = sp.run(cmd)
        if result.returncode != 0:
            msg = f'[BGFX]: msbuild return code: {result.returncode}'
            msg += f'\nstdout: {result.stdout}'
            msg += f'\nstderr: {result.stderr}'
            return cm.Result(cm.Error.BUILD_TOOL_ERROR, msg)

        # ==============================================================================================
        # Copy built libraries
        # FIXME: path depends on os and platform
        # ==============================================================================================
        lib_path: Path = self.deps[self.name].include_dir.parent / \
            '.build' / 'win64_vs2019' / 'bin'
        if not lib_path.exists():
            msg = '[BGFX]: path to compiled libraries not found'
            return cm.Result(cm.Error.FILE_MISSING, msg)

        # ==============================================================================================
        # TODO: do this dynamically, rather than hardcoding filenames
        # ==============================================================================================
        bgfx_lib_path: Path = lib_path / 'bgfxRelease.lib'
        bimg_lib_path: Path = lib_path / 'bimgRelease.lib'
        bx_lib_path: Path = lib_path / 'bxRelease.lib'

        lib_paths = [bgfx_lib_path, bimg_lib_path, bx_lib_path]
        for lib in lib_paths:
            if not lib.exists():
                msg = '[BGFX]: libraries not found'
                return cm.Result(cm.Error.FILE_MISSING, msg)

            shutil.copy2(lib, self.target_build_dir)

        # ==============================================================================================
        # Clean-up, restore old cwd
        # ==============================================================================================
        os.chdir(old_cwd)

        return super().build()

    def clean(self) -> cm.Result:
        return super().clean()
