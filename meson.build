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
# Dependencies ------------------------------------------------------------------ #
# ------------------------------------------------------------------------------- #

# Global list of dependencies
all_deps = []

if os == 'windows'

    # libfmt -------------------------------------------------------------------- #
    # --------------------------------------------------------------------------- #
    message('[fmt] Looking for dependency system-wide...')
    fmt_dep = dependency('fmt', required: false)

    if fmt_dep.found()
        all_deps += fmt_dep
    else
        include_dirs = ['deps/fmt/include']
        libs = ['-lfmt']
        link_args = ['-L../deps/fmt/bin', libs]

        fmt_d = declare_dependency(link_args: link_args, include_directories: include_dirs)
        message('[fmt] dependency found in \'deps\' directory')

        all_deps += fmt_d
    endif

    # glfw3 --------------------------------------------------------------------- #
    # --------------------------------------------------------------------------- #
    message('[glfw3] Looking for dependency system-wide...')
    glfw3_dep = dependency('glfw3', required: false)

    if glfw3_dep.found()
        all_deps += glfw3_dep
    else
        include_dirs = ['deps/glfw3/include']
        libs = ['-lopengl32', '-lgdi32', '-lglfw3']
        link_args = ['-L../deps/glfw3/bin', libs]

        glfw3_d = declare_dependency(link_args: link_args, include_directories: include_dirs)
        message('[glfw3] dependency found in \'deps\' directory')

        all_deps += glfw3_d
    endif

    # glad ---------------------------------------------------------------------- #
    # --------------------------------------------------------------------------- #
    message('[glad] Looking for dependency system-wide...')
    glad_dep = dependency('glad', required: false)

    if glad_dep.found()
        all_deps += glad_dep
    else
        include_dirs = ['deps/glad/include']

        glad_d = declare_dependency(include_directories: include_dirs)
        message('[glad] dependency found in \'deps\' directory')

        all_deps += glad_d
    endif

    # bgfx ---------------------------------------------------------------------- #
    # --------------------------------------------------------------------------- #
    message('[bgfx] Looking for dependency system-wide...')
    bgfx_dep = dependency('bgfx', required: false)

    if bgfx_dep.found()
        all_deps += bgfx_dep
    else
        include_dirs = ['deps/bgfx/include']
        libs = ['-lbgfxRelease']
        link_args = ['-L../deps/bgfx/bin', libs]

        bgfx_d = declare_dependency(link_args: link_args, include_directories: include_dirs)
        message('[bgfx] dependency found in \'deps\' directory')

        all_deps += bgfx_d
    endif

    # bimg ---------------------------------------------------------------------- #
    # --------------------------------------------------------------------------- #
    message('[bimg] Looking for dependency system-wide...')
    bimg_dep = dependency('bimg', required: false)

    if bimg_dep.found()
        all_deps += bimg_dep
    else
        include_dirs = ['deps/bimg/include']
        libs = ['-lbimgRelease']
        link_args = ['-L../deps/bimg/bin', libs]

        bimg_d = declare_dependency(link_args: link_args, include_directories: include_dirs)
        message('[bimg] dependency found in \'deps\' directory')

        all_deps += bimg_d
    endif

    # bx ------------------------------------------------------------------------ #
    # --------------------------------------------------------------------------- #
    message('[bx] Looking for dependency system-wide...')
    bx_dep = dependency('bx', required: false)

    if bx_dep.found()
        all_deps += bx_dep
    else
        include_dirs = ['deps/bx/include']
        libs = ['-lbxRelease']
        link_args = ['-L../deps/bx/bin', libs]

        bx_d = declare_dependency(link_args: link_args, include_directories: include_dirs)
        message('[bx] dependency found in \'deps\' directory')

        all_deps += bx_d
    endif

    # spdlog -------------------------------------------------------------------- #
    # --------------------------------------------------------------------------- #
    message('[spdlog] Looking for dependency system-wide...')
    spdlog_dep = dependency('spdlog', required: false)

    if spdlog_dep.found()
        all_deps += spdlog_dep
    else
        include_dirs = ['deps/spdlog/include']

        spdlog_d = declare_dependency(include_directories: include_dirs)
        message('[spdlog] dependency found in \'deps\' directory')

        all_deps += spdlog_d
    endif
else
    # libfmt -------------------------------------------------------------------- #
    # --------------------------------------------------------------------------- #
    message('[fmt] Looking for dependency system-wide...')
    fmt_dep = dependency('fmt', required: true)
    all_deps += fmt_dep

    # glfw3 --------------------------------------------------------------------- #
    # --------------------------------------------------------------------------- #
    message('[glfw3] Looking for dependency system-wide...')
    glfw3_dep = dependency('glfw3', required: true)
    all_deps += glfw3_dep

    # glad ---------------------------------------------------------------------- #
    # --------------------------------------------------------------------------- #
    message('[glad] Looking for dependency system-wide...')
    glad = dependency('glad', required: true)
    all_deps += glad_dep

    # bgfx ---------------------------------------------------------------------- #
    # --------------------------------------------------------------------------- #
    message('[bgfx] Looking for dependency system-wide...')
    bgfx_dep = dependency('bgfx', required: true)
    all_deps += bgfx_dep

    # bimg ---------------------------------------------------------------------- #
    # --------------------------------------------------------------------------- #
    message('[bimg] Looking for dependency system-wide...')
    bimg_dep = dependency('bimg', required: true)
    all_deps += bimg_dep

    # bx ------------------------------------------------------------------------ #
    # --------------------------------------------------------------------------- #
    message('[bx] Looking for dependency system-wide...')
    bx_dep = dependency('bx', required: true)
    all_deps += bx_dep

    # spdlog -------------------------------------------------------------------- #
    # --------------------------------------------------------------------------- #
    message('[spdlog] Looking for dependency system-wide...')
    spdlog_dep = dependency('spdlog', required: true)
    all_deps += spdlog_dep
endif

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
