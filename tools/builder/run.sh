#!/bin/sh

poetry run python src/build.py --action "build" --root_path "../../." --deps "bgfx" "spdlog" "fmt" "glfw3"
