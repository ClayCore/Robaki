#!/bin/sh

pushd tools/py-cppbuild
pwd
poetry run python src/py-cppbuild.py --action "build" --root_path "../../." --deps $1
popd
