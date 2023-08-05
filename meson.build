project(
    'worming',
    'cpp', 'c',
    version: '0.1.0-dev',
    default_options: [
        'cpp_std=c++20',
        'warning_level=3',
        'optimization=3',
    ]
)

# --------------------------------------------------------------------------- #
# Variables ----------------------------------------------------------------- #
# --------------------------------------------------------------------------- #
compiler = meson.get_compiler('cpp')
os = host_machine.system()

shared_include_directories = [
    include_directories('src'),
]

sources = [
    'vendor/glad/src/glad.c',
    'src/event/event.cpp',
    'src/platform/window.cpp',
    'src/platform/input.cpp',
    'src/platform/platform.cpp',
    'src/main.cpp',
]

# --------------------------------------------------------------------------- #
# Dependencies -------------------------------------------------------------- #
# --------------------------------------------------------------------------- #

# Global list of dependencies
all_deps = []

if os == 'windows'

    # libfmt
    message('[libfmt] Looking for dependency system-wide...')
    fmt_dep = dependency('libfmt', required: false)

    if fmt_dep.found()
        all_deps += fmt_dep
    else
        fmt_d_include_dirs = ['vendor/fmt/include']
        fmt_d = declare_dependency(include_directories: fmt_d_include_dirs)

        message('[libfmt] dependency found in \'vendor\' directory')

        all_deps += fmt_d
    endif

    # glfw3
    message('[glfw3] Looking for dependency system-wide...')
    glfw3_dep = dependency('glfw3', required: false)

    if glfw3_dep.found()
        all_deps += glfw3_dep
    else
        glfw3_d_include_dirs = ['vendor/glfw3/include']

        glfw3_d_libs = ['-lopengl32', '-lgdi32', '-lglfw3']
        glfw3_d_link_args = ['-L../vendor/glfw3/lib', glfw3_d_libs]

        glfw3_d = declare_dependency(link_args: glfw3_d_link_args, include_directories: glfw3_d_include_dirs)

        message('[glfw3] dependency found in \'vendor\' directory')

        all_deps += glfw3_d
    endif

    # glad
    message('[glad] Looking for dependency system-wide...')
    glad_dep = dependency('glad', required: false)

    if glad_dep.found()
        all_deps += glad_dep
    else
        glad_d_include_dirs = ['vendor/glad/include']

        glad_d = declare_dependency(include_directories: glad_d_include_dirs)

        message('[glad] dependency found in \'vendor\' directory')

        all_deps += glad_d
    endif

    # bgfx
    message('[bgfx] Looking for dependency system-wide...')
    bgfx_dep = dependency('bgfx', required: false)

    if bgfx_dep.found()
        all_deps += bgfx_dep
    else
        bgfx_d_include_dirs = ['vendor/bgfx/include']

        bgfx_d_libs = ['-lbgfxRelease']
        bgfx_d_link_args = ['-L../vendor/bgfx/.build/win64_vs2019/bin', bgfx_d_libs]

        bgfx_d = declare_dependency(link_args: bgfx_d_link_args, include_directories: bgfx_d_include_dirs)

        message('[bgfx] dependency found in \'vendor\' directory')

        all_deps += bgfx_d
    endif

    # bimg
    message('[bimg] Looking for dependency system-wide...')
    bimg_dep = dependency('bimg', required: false)

    if bimg_dep.found()
        all_deps += bimg_dep
    else
        bimg_d_include_dirs = ['vendor/bimg/include']

        bimg_d_libs = ['-lbimgRelease']
        bimg_d_link_args = ['-L../vendor/bimg/.build/win64_vs2019/bin', bimg_d_libs]

        bimg_d = declare_dependency(link_args: bimg_d_link_args, include_directories: bimg_d_include_dirs)

        message('[bimg] dependency found in \'vendor\' directory')

        all_deps += bimg_d
    endif

    # bx
    message('[bx] Looking for dependency system-wide...')
    bx_dep = dependency('bx', required: false)

    if bx_dep.found()
        all_deps += bx_dep
    else
        bx_d_include_dirs = ['vendor/bx/include']

        bx_d_libs = ['-lbxRelease']
        bx_d_link_args = ['-L../vendor/bx/.build/win64_vs2019/bin', bx_d_libs]

        bx_d = declare_dependency(link_args: bx_d_link_args, include_directories: bx_d_include_dirs)

        message('[bx] dependency found in \'vendor\' directory')

        all_deps += bx_d
    endif
else
    message('[libfmt] Looking for dependency system-wide...')
    fmt_dep = dependency('libfmt', required: true)
    all_deps += fmt_dep

    message('[glfw3] Looking for dependency system-wide...')
    glfw3_dep = dependency('glfw3', required: true)
    all_deps += glfw3_dep

    message('[glad] Looking for dependency system-wide...')
    glad = dependency('glad', required: true)
    all_deps += glad_dep

    message('[bgfx] Looking for dependency system-wide...')
    bgfx_dep = dependency('bgfx', required: true)
    all_deps += bgfx_dep

    message('[bimg] Looking for dependency system-wide...')
    bimg_dep = dependency('bimg', required: true)
    all_Deps += bimg_dep

    message('[bx] Looking for dependency system-wide...')
    bx_dep = dependency('bx', required: true)
    all_Deps += bx_dep
endif

# --------------------------------------------------------------------------- #
# Linking into executable --------------------------------------------------- #
# --------------------------------------------------------------------------- #

executable(
    'worming',
    sources,
    include_directories: shared_include_directories,
    dependencies: all_deps,
    win_subsystem: 'console'
)
