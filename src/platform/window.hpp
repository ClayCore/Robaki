/** @file window.hpp */

#pragma once

// module includes
#include "event/emitter.hpp"
#include "input.hpp"
#include "managers/event_manager.hpp"
#include "managers/state_manager.hpp"
#include "util/util.hpp"

// bgfx and glfw includes
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include "GLFW/glfw3.h"
#include "bgfx/bgfx.h"
#include "glad/glad.h"

namespace platform
{
    namespace details
    {
        using StateDispatcher = decltype(managers::EventManager::instance().get_dispatcher());

        struct WindowDeleter
        {
            inline auto operator()(GLFWwindow *window) -> void
            {
                glfwDestroyWindow(window);
                glfwTerminate();
            }
        };
    }  // namespace details

    class Window
    {
    private:
        class Emitter : public event::Emitter<Window::Emitter>
        {
        public:
            auto emit(event::Event const &event) -> void
            {
                for (auto const &dispatcher : m_dispatchers) {
                    dispatcher->dispatch(event);
                }
            }
        };

        u32 m_width;
        u32 m_height;
        std::string m_title;

        std::unique_ptr<GLFWwindow *, details::WindowDeleter> m_handle;
        Emitter m_emitter;

        static auto error_callback(i32, char const *) -> void;
        static auto size_callback(GLFWwindow *, i32, i32) -> void;
        static auto cursor_callback(GLFWwindow *, f64, f64) -> void;
        static auto key_callback(GLFWwindow *, i32, i32, i32, i32) -> void;
        static auto mouse_callback(GLFWwindow *, i32, i32, i32) -> void;

    public:
        Window();
    };
}  // namespace platform
