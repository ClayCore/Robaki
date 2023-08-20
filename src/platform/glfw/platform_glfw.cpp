/** @file platform_glfw.cpp */

// module includes
#include "platform_glfw.hpp"
#include "GLFW/glfw3.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

namespace platform::glfw
{
    static auto callback_error(i32 err, char const *msg)
    {
        // todo logging
        fmt::print(stderr, "[GLFW/ERROR]: code: {}, msg: {}\n", err, msg);
    }

    auto Window::set_flags(platform::Window::WindowFlags flags) -> void
    {
        glfwWindowHint(GLFW_RESIZABLE, flags.resizeable);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, flags.version_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, flags.version_minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, flags.profile);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, flags.forward_compat);
    }

    auto Window::init_glfw(platform::Window::WindowFlags flags, Vec2<i32> const &size) -> void
    {
        if (glfwInit() == 0) {
            fmt::print(stderr, "[GLFW/ERROR]: Failed to initialize\n");
        }

        i32 screen_count { 0U };
        GLFWmonitor **screens { glfwGetMonitors(&screen_count) };
        GLFWmonitor *screen { screens[0] };

        // todo update state and platform settings
        GLFWvidmode const *video_mode = glfwGetVideoMode(screen);
        Vec2<i32> screen_pos          = {};
        glfwGetMonitorPos(screen, &screen_pos.x(), &screen_pos.y());

        // start with screen invis
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_handle = std::unique_ptr<GLFWwindow, detail::WindowDeleter>(
            glfwCreateWindow(size.x(), size.y(), "Worming v0.0.1", nullptr, nullptr));

        if (!m_handle) {
            fmt::print(stderr, "[GLFW/ERROR]: Failed to create window\n");
            glfwTerminate();
            std::exit(1);
        }

        glfwDefaultWindowHints();
        glfwSetWindowPos(m_handle.get(),
                         screen_pos.x() + (video_mode->width - size.x()) / 2,
                         screen_pos.y() + (video_mode->height - size.y()) / 2);
        glfwSetWindowUserPointer(m_handle.get(), this);
        glfwShowWindow(m_handle.get());
    }

    Window::Window(Vec2<i32> const &size)
    {
        platform::Window::WindowFlags flags;
        this->init_glfw(flags, size);

        bgfx::renderFrame();
    }
}  // namespace platform::glfw
