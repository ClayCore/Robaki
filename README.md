# Worming

Real-time Worms clone.

<!-- TODO: Description -->

## Getting started

### Prerequisites

The following libraries must be installed in the vendor directory.
Please refer to the [meson.build](meson.build) configuration file for more information.

-   [bgfx](https://github.com/bkaradzic/bgfx),
-   [bimg](https://github.com/bkaradzic/bimg),
-   [bx](https://github.com/bkaradzic/bx),
-   [{fmt}](https://github.com/fmtlib/fmt),
-   [glfw3](https://github.com/glfw/glfw),
-   [glad](https://glad.dav1d.de/)

### Building

This project makes use of the [meson](https://mesonbuild.com/index.html) build system and a Makefile is supplied which invokes meson and its targets

Invoke the `build` target from command line.
`$ make build`

Other targets are also defined by the [Makefile](Makefile). Refer to it for more information.

## License

This project is under the BSD 3-clause License. See [LICENSE](LICENSE) for details.
