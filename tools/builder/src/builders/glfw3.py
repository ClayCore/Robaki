from pathlib import Path
import shutil

from . import common as cm
import subprocess as sp
import os
import shlex


class GLFW3Builder(cm.Builder):
    def __init__(self, root_path: Path, deps: dict):
        super().__init__(root_path, deps, 'glfw3')

    def prepare(self) -> cm.Result:
        # ==============================================================================================
        # glfw3 must be built
        # ==============================================================================================

        # ==============================================================================================
        # Create include directory and copy headers
        # ==============================================================================================
        if not self.target_include_dir.exists():
            self.target_include_dir.mkdir(parents=True)

        cm.Builder.copytree(self.include_dir, self.target_include_dir)
        if not self.target_include_dir.exists():
            msg = f'[GLFW3]: failed to transact copy to \'{str(self.target_include_dir)}\''
            return cm.Result(cm.Error.FILE_COPY_FAILED, msg)

        # ==============================================================================================
        # Create build directory
        # ==============================================================================================
        if not self.target_build_dir.exists():
            self.target_build_dir.mkdir(parents=True)

        if not self.target_build_dir.exists():
            msg = f'[GLFW3]: failed to create directory \'{str(self.target_build_dir)}\''
            return cm.Result(cm.Error.FILE_MISSING, msg)

        return super().prepare()

    def build(self) -> cm.Result:
        # ==============================================================================================
        # Save current cwd
        # ==============================================================================================
        old_cwd: str = os.getcwd()

        # ==============================================================================================
        # Build glfw3 in target build directory
        # ==============================================================================================
        cwd: Path = self.target_build_dir
        os.chdir(str(cwd))

        # ==============================================================================================
        # Run cmake to generate build configurations
        # ==============================================================================================
        cmd = shlex.split(f'cmake ../../../vendor/{self.name}')
        result: sp.CompletedProcess = sp.run(cmd)
        if result.returncode != 0:
            msg = f'[GLFW3]: cmake return code: {result.returncode}'
            msg += f'\nstdout: {result.stdout}'
            msg += f'\nstderr: {result.stderr}'
            return cm.Result(cm.Error.BUILD_TOOL_ERROR, msg)

        # ==============================================================================================
        # Use 'msbuild' to build it
        # FIXME: msbuild not used on any platform except windows
        # ==============================================================================================
        cmd = ['msbuild', 'GLFW.sln', '/t:GLFW3\\glfw', '/clp:ErrorsOnly',
               '/p:Configuration=Release', '/p:Platform=x64']
        result: sp.CompletedProcess = sp.run(cmd)
        if result.returncode != 0:
            msg = f'[GLFW3]: msbuild return code: {result.returncode}'
            msg += f'\nstdout: {result.stdout}'
            msg += f'\nstderr: {result.stderr}'
            return cm.Result(cm.Error.BUILD_TOOL_ERROR, msg)

        # ==============================================================================================
        # Copy built library
        # ==============================================================================================
        lib_path: Path = self.target_build_dir / 'src' / 'Release'
        if not lib_path.exists():
            msg = '[GLFW3]: path to compiler library directory not found'
            return cm.Result(cm.Error.FILE_MISSING, msg)

        glfw_lib_path: Path = lib_path / 'glfw3.lib'
        if not glfw_lib_path.exists():
            msg = '[GLFW3]: path to compiler library not found'
            return cm.Result(cm.Error.FILE_MISSING, msg)

        shutil.copy2(glfw_lib_path, self.target_build_dir)

        # ==============================================================================================
        # Clean-up, restore old cwd
        # and remove all other directories and files associated with the build
        # ==============================================================================================
        os.chdir(old_cwd)

        files: list[str] = []
        for path in self.target_build_dir.glob('*'):
            if path != glfw_lib_path:
                files.append(str(path))

        for path in files:
            if os.path.isfile(path) and not path.endswith('.lib'):
                p = Path(path).unlink()
            if os.path.isdir(path):
                shutil.rmtree(path)

        return super().build()

    def clean(self) -> cm.Result:
        return super().clean()
