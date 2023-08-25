from dataclasses import dataclass
from pathlib import Path


@dataclass
class Dependency(object):
    name: str
    root_path: Path

    build_dir: Path
    include_dir: Path

    target_build_dir: Path
    target_include_dir: Path

    built: bool

    def exists(self) -> bool:
        if self.build_dir.parent.exists():
            return True

        return False

    def is_built(self) -> bool:
        tbd = self.target_build_dir.exists()
        tid = self.target_include_dir.exists()

        if self.built and tbd and tid:
            return True

        return False

    @staticmethod
    def create(name: str, root_path: Path):
        build_dir: Path = root_path / 'vendor' / name / 'bin'
        include_dir: Path = root_path / 'vendor' / name / 'include'

        target_build_dir: Path = root_path / 'deps' / name / 'bin'
        target_include_dir: Path = root_path / 'deps' / name / 'include'

        return Dependency(
            name, root_path,
            build_dir, include_dir,
            target_build_dir, target_include_dir,
            built=False
        )
