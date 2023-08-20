# Robaki

[![](https://tokei.rs/b1/github/ClayCore/Robaki)](https://github.com/ClayCore/Robaki)

Real-time Worms clone.

## Getting started

### Documentation

Documentation can be generated using [Doxygen](https://www.doxygen.nl/index.html).
The repository also comes with [doxygen-awesome-css](https://github.com/jothepro/doxygen-awesome-css).

Refer to [Doxyfile](Doxyfile) for configuration.

### Prerequisites

The following libraries must be installed system-wide or in the vendor directory.

-   [bgfx](https://github.com/bkaradzic/bgfx),
-   [bimg](https://github.com/bkaradzic/bimg),
-   [bx](https://github.com/bkaradzic/bx),
-   [{fmt}](https://github.com/fmtlib/fmt),
-   [glfw3](https://github.com/glfw/glfw),
-   [glad](https://glad.dav1d.de/),
-   [spdlog](https://github.com/gabime/spdlog)

Meson will try its best to find them using whatever package manager thats available in your system,
however if it fails, you can install those libraries locally.

Dependencies also have to be installed using the following directory structure:

```
/vendor/
├─<bimg>
│  ├─include/
│  │  ├─<headers>
│  ├─lib/
│  │  ├─<binaries>
```

Please refer to the [meson.build](meson.build) configuration file for more information.

### Building

This project makes use of the [meson](https://mesonbuild.com/index.html) build system and a Makefile is supplied which invokes meson and its targets

Invoke the `build` target from command line.
`$ make build`

Other targets are also defined by the [Makefile](Makefile). Refer to it for more information.

## License

This project is under the BSD 3-clause License. See [LICENSE](LICENSE) for details.
