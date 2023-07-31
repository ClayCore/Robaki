/** @file window.cpp */

// module includes
#include "window.hpp"

namespace platform
{
    Window::Window()
    {
        if (!glfwInit()) {
            fmt::print(stderr, "[GLFW/ERROR] Failed to initialize\n");
            std::exit(1);
        }
    }
}  // namespace platform
