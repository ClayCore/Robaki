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

shared_include_directories = [
    include_directories('src'),
]

sources = [
    'vendor/glad/src/glad.c',
    'src/main.cpp',
]

# --------------------------------------------------------------------------- #
# Dependencies -------------------------------------------------------------- #
# --------------------------------------------------------------------------- #

all_deps = []

# {fmt} dependency
fmt_include_dirs = ['vendor/fmt/include']

fmt_d = declare_dependency(include_directories: fmt_include_dirs)
message('fmt dependency in local vendor directory')

all_deps += fmt_d

# glfw3 dependency
glfw3_include_dirs = ['vendor/glfw3/include']
glfw3_libs = ['-lopengl32', '-lgdi32', '-lglfw3']
glfw3_link_args = ['-L../vendor/glfw3/lib', glfw3_libs]

glfw3_d = declare_dependency(link_args: glfw3_link_args, include_directories: glfw3_include_dirs)
message('glfw3 dependency in local vendor directory')

all_deps += glfw3_d

# glad dependency
glad_include_dirs = ['vendor/glad/include']

glad_d = declare_dependency(include_directories: glad_include_dirs)
message('glad dependency in local vendor directory')

all_deps += glad_d

# bgfx dependency
bgfx_include_dirs = ['vendor/bgfx/include']
bgfx_libs = ['-lbgfxRelease']
bgfx_link_args = ['-L../vendor/bgfx/.build/win64_vs2019/bin', bgfx_libs]

bgfx_d = declare_dependency(link_args: bgfx_link_args, include_directories: bgfx_include_dirs)
message('bgfx dependency in local vendor directory')

all_deps += bgfx_d

# bimg dependency
bimg_include_dirs = ['vendor/bimg/include']
bimg_libs = ['-lbimgRelease']
bimg_link_args = ['-L../vendor/bgfx/.build/win64_vs2019/bin', bimg_libs]

bimg_d = declare_dependency(link_args: bimg_link_args, include_directories: bimg_include_dirs)
message('bimg dependency in local vendor directory')

all_deps += bimg_d

# bx dependency
bx_include_dirs = ['vendor/bx/include']
bx_libs = ['-lbxRelease']
bx_link_args = ['-L../vendor/bgfx/.build/win64_vs2019/bin', bx_libs]

bx_d = declare_dependency(link_args: bx_link_args, include_directories: bx_include_dirs)
message('bx dependency in local vendor directory')

all_deps += bx_d

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
