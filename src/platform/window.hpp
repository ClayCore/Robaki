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
        struct WindowHints
        {
            i32 resizeable     = { GL_FALSE };
            i32 version_major  = { 4 };
            i32 version_minor  = { 1 };
            i32 profile        = { GLFW_OPENGL_CORE_PROFILE };
            i32 forward_compat = { GL_TRUE };
        };

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

        i32 m_width{ 800 };
        i32 m_height{ 600 };
        std::string m_title{ "Worming v0.0.1" };

        u64 m_last_second;

        u64 m_fps            = { 0 };
        u64 m_frame_delta    = { 0 };
        u64 m_frames         = { 0 };
        u64 m_last_frame     = { 0 };
        u64 m_tick_remainder = { 0 };
        u64 m_ticks          = { 0 };
        u64 m_tps            = { 0 };

        std::unique_ptr<GLFWwindow, details::WindowDeleter> m_handle;
        Emitter m_emitter;

        static auto set_hints(WindowHints hints) -> void
        {
            glfwWindowHint(GLFW_RESIZABLE, hints.resizeable);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, hints.version_major);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, hints.version_minor);
            glfwWindowHint(GLFW_OPENGL_PROFILE, hints.profile);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, hints.forward_compat);
        }

        auto set_callbacks() -> void
        {
        }

        auto init_glfw(WindowHints hints) -> void
        {
            if (glfwInit() == 0) {
                fmt::print(stderr, "[GLFW/ERROR] Failed to initialize\n");
                glfwTerminate();
                std::exit(1);
            }

            Window::set_hints(hints);

            m_handle = std::unique_ptr<GLFWwindow, details::WindowDeleter>(
                glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr));

            glfwMakeContextCurrent(m_handle.get());

            this->set_callbacks();

            if (gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)) == 0) {
                fmt::print(stderr, "[GLAD/ERROR] Failed to initialize\n");
                glfwTerminate();
                std::exit(1);
            }

            glfwSwapInterval(1);
        }

        auto main_loop() -> void
        {
            while (glfwWindowShouldClose(m_handle.get()) == 0) {
                auto now = util::time::get_time_ns();

                m_frame_delta = now - m_last_frame;
                m_last_frame  = now;

                if ((now - m_last_second) > util::time::NS_PER_SEC) {
                    m_fps = m_frames;
                    m_tps = m_ticks;

                    m_frames = 0;
                    m_ticks  = 0;

                    m_last_second = now;

                    fmt::print("FPS: {} | TPS: {}\n", m_fps, m_tps);
                }

                u64 const NS_PER_TICK = (util::time::NS_PER_SEC / 60);
                u64 tick_time         = m_frame_delta + m_tick_remainder;
                while (tick_time > NS_PER_TICK) {
                    m_ticks += 1;

                    tick_time -= NS_PER_TICK;
                }

                m_tick_remainder = std::max(tick_time, 0ULL);

                m_frames += 1;

                glfwSwapBuffers(m_handle.get());
                glfwPollEvents();
            }
        }

    public:
        Window()
        {
            auto now = util::time::get_time_ns();

            m_last_frame  = now;
            m_last_second = now;

            WindowHints hints;
            this->init_glfw(hints);
            this->main_loop();
        }
    };
}  // namespace platform
