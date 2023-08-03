/** @file window.cpp */

// module includes
#include "window.hpp"

namespace platform
{
    auto Window::error_callback(i32 code, char const *desc) -> void
    {
    }

    auto Window::size_callback(GLFWwindow *, i32 width, i32 height) -> void
    {
    }

    auto Window::cursor_callback(GLFWwindow *, f64 xpos, f64 ypos) -> void
    {
    }

    auto Window::key_callback(GLFWwindow *, i32 key, i32, i32 action, i32) -> void
    {
    }

    auto Window::mouse_callback(GLFWwindow *, i32 button, i32 action, i32) -> void
    {
    }

    Window::Window()
    {
        if (glfwInit() == 0) {
            fmt::print(stderr, "[GLFW/ERROR] Failed to initialize\n");
            std::exit(1);
        }
    }
}  // namespace platform
