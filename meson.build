project(
    'worming',
    'cpp', 'c',
    version: '0.1.0-dev',
    default_options: [
        'cpp_std=c++20',
        'warning_level=3',
        'optimization=3',
        'b_pch=true'
    ]
)

# ------------------------------------------------------------------------------- #
# Variables --------------------------------------------------------------------- #
# ------------------------------------------------------------------------------- #
compiler = meson.get_compiler('cpp')
os = host_machine.system()

shared_include_directories = [
    include_directories('src'),
]

sources = [
    'deps/glad/src/glad.c',
    'src/assets/io/file.cpp',
    'src/event/event.cpp',
    'src/memory/fallback_allocator.cpp',
    'src/memory/free_list.cpp',
    'src/memory/null_allocator.cpp',
    'src/memory/stack_allocator.cpp',
    'src/platform/glfw/platform_glfw.cpp',
    'src/platform/input.cpp',
    'src/platform/platform.cpp',
    'src/platform/window.cpp',
    'src/main.cpp',
]

# ------------------------------------------------------------------------------- #
# Custom target for running `py-cppbuild` --------------------------------------- #
# ------------------------------------------------------------------------------- #

poetry = find_program('poetry', required: true)

# ------------------------------------------------------------------------------- #
# Dependencies ------------------------------------------------------------------ #
# ------------------------------------------------------------------------------- #

# Global list of dependencies
all_deps = []

# fmt ----------------------------------------------------------------------- #
# --------------------------------------------------------------------------- #
message('[fmt] Looking for dependency system-wide...')
fmt_dep = dependency('fmt', version: '10.1.0')
if fmt_dep.found()
    all_deps += fmt_dep
else
    if poetry.found()
        message('[fmt] generating build using \'py-cppbuild\'')
        deps = ['fmt']
        args = ['--action', 'build', '--root_path', '.', '--deps', deps]
        cmd = ['run', 'python', 'tools/py-cppbuild', args]
        run_command(poetry, cmd)

        include_directories = ['deps/fmt/include']
        libs = ['-lfmt']
        link_args = ['-L../deps/fmt/bin', libs]

        fmt_d = declare_dependency(link_args, include_directories)
        message('[fmt] dependency found in \'deps\' directory')

        all_deps += fmt_d
    endif
endif

# glfw3 --------------------------------------------------------------------- #
# --------------------------------------------------------------------------- #
message('[glfw3] Looking for dependency system-wide...')
glfw3_dep = dependency('glfw', version: '3.3.8')
if glfw3_dep.found()
    all_deps += glfw3_dep
else
    if poetry.found()
        message('[glfw3] generating build using \'py-cppbuild\'')
        deps = ['glfw3']
        args = ['--action', 'build', '--root_path', '.', '--deps', deps]
        cmd = ['run', 'python', 'tools/py-cppbuild', args]
        run_command(poetry, cmd)

        include_directories = ['deps/glfw3/include']
        libs = ['-lglfw3']
        link_args = ['-L../deps/glfw3/bin', libs]

        glfw3_d = declare_dependency(link_args, include_directories)
        message('[glfw3] dependency found in \'deps\' directory')

        all_deps += glfw3_d
    endif
endif

# bgfx ---------------------------------------------------------------------- #
# --------------------------------------------------------------------------- #
message('[bgfx] Looking for dependency system-wide...')
bgfx_dep = dependency('bgfx')
if bgfx_dep.found()
    all_deps += bgfx_dep
else
    if poetry.found()
        message('[bgfx] generating build using \'py-cppbuild\'')
        deps = ['bgfx']
        args = ['--action', 'build', '--root_path', '.', '--deps', deps]
        cmd = ['run', 'python', 'tools/py-cppbuild', args]
        run_command(poetry, cmd)

        include_directories = ['deps/bgfx/include']
        libs = ['-lbgfxRelease']
        link_args = ['-L../deps/bgfx/bin', libs]

        bgfx_d = declare_dependency(link_args, include_directories)
        message('[bgfx] dependency found in \'deps\' directory')

        all_deps += bgfx_d
    endif
endif

# bimg ---------------------------------------------------------------------- #
# --------------------------------------------------------------------------- #
message('[bimg] Looking for dependency system-wide...')
bimg_dep = dependency('bimg')
if bimg_dep.found()
    all_deps += bimg_dep
else
    if poetry.found()
        include_directories = ['deps/bimg/include']
        libs = ['-lbimgRelease']
        link_args = ['-L../deps/bimg/bin', libs]

        bimg_d = declare_dependency(link_args, include_directories)
        message('[bimg] dependency found in \'deps\' directory')

        all_deps += bimg_d
    endif
endif

# bx ------------------------------------------------------------------------ #
# --------------------------------------------------------------------------- #
message('[bx] Looking for dependency system-wide...')
bx_dep = dependency('bx')
if bx_dep.found()
    all_deps += bx_dep
else
    if poetry.found()
        include_directories = ['deps/bx/include']
        libs = ['-lbxRelease']
        link_args = ['-L../deps/bx/bin', libs]

        bx_d = declare_dependency(link_args, include_directories)
        message('[bx] dependency found in \'deps\' directory')

        all_deps += bx_d
    endif
endif

# spdlog -------------------------------------------------------------------- #
# --------------------------------------------------------------------------- #
message('[spdlog] Looking for dependency system-wide...')
spdlog_dep = dependency('spdlog', version: '1.12.0')
if spdlog_dep.found()
    all_deps += spdlog_dep
else
    if poetry.found()
        message('[spdlog] generating build using \'py-cppbuild\'')
        deps = ['spdlog']
        args = ['--action', 'build', '--root_path', '.', '--deps', deps]
        cmd = ['run', 'python', 'tools/py-cppbuild', args]
        run_command(poetry, cmd)

        include_directories = ['deps/spdlog/include']

        spdlog_d = declare_dependency(include_directories)
        message('[spdlog] dependency found in \'deps\' directory')

        all_deps += spdlog_d
    endif
endif

# glad ---------------------------------------------------------------------- #
# --------------------------------------------------------------------------- #
include_directories = ['deps/glad/include']

glad_dep = declare_dependency(include_directories)
message('[glad] dependency found in \'deps\' directory')

all_deps += glad_dep

# ------------------------------------------------------------------------------- #
# Linking into executable ------------------------------------------------------- #
# ------------------------------------------------------------------------------- #

executable(
    'worming',
    sources,
    include_directories: shared_include_directories,
    dependencies: all_deps,
    win_subsystem: 'console',
    cpp_pch: 'include/pch/common_pch.hpp'
)
