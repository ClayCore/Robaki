@echo off
setlocal enabledelayedexpansion
cd tools/py-cppbuild
set msg_a=Current Directory: 
set msg_b=%cd%
set msg_c=%msg_a%%msg_b%
echo %msg_c%
poetry run python src/py-cppbuild.py --action "build" --root_path "../../." --deps %1
popd
