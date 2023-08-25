from pathlib import Path
import shutil

from . import common as cm
import subprocess as sp
import os
import shlex


class SFMLBuilder(cm.Builder):
    def __init__(self, root_path: Path, deps: dict):
        super().__init__(root_path, deps, 'sfml')

    def prepare(self) -> cm.Result:
        # ==============================================================================================
        # sfml must be built
        # ==============================================================================================

        # ==============================================================================================
        # Create include directory and copy headers
        # ==============================================================================================
        if not self.target_include_dir.exists():
            self.target_include_dir.mkdir(parents=True)

        cm.Builder.copytree(self.include_dir, self.target_include_dir)
        if not self.target_include_dir.exists():
            msg = f'[SFML]: failed to transact copy to \'{str(self.target_include_dir)}\''
            return cm.Result(cm.Error.FILE_COPY_FAILED, msg)

        # ==============================================================================================
        # Create build directory
        # ==============================================================================================
        if not self.target_build_dir.exists():
            self.target_build_dir.mkdir(parents=True)

        if not self.target_build_dir.exists():
            msg = f'[SFML]: failed to create directory \'{str(self.target_build_dir)}\''
            return cm.Result(cm.Error.FILE_MISSING, msg)

        return super().prepare()

    def build(self) -> cm.Result:
        # ==============================================================================================
        # Save current cwd
        # ==============================================================================================
        old_cwd: str = os.getcwd()

        # ==============================================================================================
        # Build SFML in target build directory
        # ==============================================================================================
        cwd: Path = self.target_build_dir
        os.chdir(str(cwd))

        # ==============================================================================================
        # Run cmake to generate build configurations
        # ==============================================================================================
        cmd = shlex.split(
            f'cmake ../../../vendor/{self.name} -DCMAKE_BUILD_TYPE=Release')
        result: sp.CompletedProcess = sp.run(cmd)
        if result.returncode != 0:
            msg = f'[SFML]: cmake return code: {result.returncode}'
            msg += f'\nstdout: {result.stdout}'
            msg += f'\nstderr: {result.stderr}'
            return cm.Result(cm.Error.BUILD_TOOL_ERROR, msg)

        # ==============================================================================================
        # Use 'msbuild' to build it
        # FIXME: msbuild not used on any platform except windows
        # ==============================================================================================
        cmd = ['msbuild', 'SFML.sln', '/clp:ErrorsOnly',
               '/t:CMake\\ALL_BUILD', '/p:Configuration=Release', '/p:Platform=x64']
        result: sp.CompletedProcess = sp.run(cmd)
        if result.returncode != 0:
            msg = f'[SFML]: msbuild return code: {result.returncode}'
            msg += f'\nstdout: {result.stdout}'
            msg += f'\nstderr: {result.stderr}'
            return cm.Result(cm.Error.BUILD_TOOL_ERROR, msg)

        # ==============================================================================================
        # Copy built library
        # ==============================================================================================
        lib_path: Path = self.target_build_dir / 'lib' / 'Release'
        if not lib_path.exists():
            msg = '[SFML]: path to compiler library directory not found'
            return cm.Result(cm.Error.FILE_MISSING, msg)

        sfml_audio_lib: Path = lib_path / 'sfml-audio.lib'
        sfml_graphics_lib: Path = lib_path / 'sfml-graphics.lib'
        sfml_main_lib: Path = lib_path / 'sfml-main.lib'
        sfml_network_lib: Path = lib_path / 'sfml-network.lib'
        sfml_system_lib: Path = lib_path / 'sfml-system.lib'
        sfml_window_lib: Path = lib_path / 'sfml-window.lib'

        lib_paths = [sfml_audio_lib, sfml_graphics_lib, sfml_main_lib,
                     sfml_network_lib, sfml_system_lib, sfml_window_lib]

        for lib in lib_paths:
            if not lib.exists():
                msg = '[SFML]: path to compiler library not found'
                return cm.Result(cm.Error.FILE_MISSING, msg)

            shutil.copy2(lib, self.target_build_dir)

        # ==============================================================================================
        # Clean-up, restore old cwd
        # and remove all other directories and files associated with the build
        # ==============================================================================================
        os.chdir(old_cwd)

        files: list[str] = []
        for path in self.target_build_dir.glob('*'):
            for lib in lib_paths:
                if path != lib:
                    files.append(str(path))

        for path in files:
            if os.path.isfile(path) and not path.endswith('.lib'):
                p = Path(path).unlink()
            if os.path.isdir(path):
                shutil.rmtree(path)

        return super().build()

    def clean(self) -> cm.Result:
        return super().clean()
