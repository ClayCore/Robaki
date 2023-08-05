/** @file window.cpp */

// module includes
#include "window.hpp"

namespace platform
{
    auto Window::Emitter::emit(event::Event const &event) -> void
    {
        for (auto const &dispatcher : m_dispatchers) {
            dispatcher->dispatch(event);
        }
    }

    auto Window::set_flags(Window::WindowFlags flags) -> void
    {
        glfwWindowHint(GLFW_RESIZABLE, flags.resizeable);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, flags.version_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, flags.version_minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, flags.profile);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, flags.forward_compat);
    }

    auto Window::set_callbacks() -> void
    {
        // TODO
    }

    auto Window::tick() -> void
    {
        m_ticks += 1;

        event::Event event(event::EventType::EngineTick);
        m_emitter.emit(event);
    }

    auto Window::update() -> void
    {
        event::Event event(event::EventType::EngineUpdate);
        m_emitter.emit(event);
    }

    auto Window::render() -> void
    {
        m_frames += 1;

        event::Event event(event::EventType::EngineRender);
        m_emitter.emit(event);
    }

    auto Window::init_glfw(Window::WindowFlags flags) -> void
    {
        if (glfwInit() == 0) {
            fmt::print(stderr, "[GLFW/ERROR] Failed to initialize\n");
            glfwTerminate();
            std::exit(1);
        }

        Window::set_flags(flags);

        m_handle = std::unique_ptr<GLFWwindow, details::WindowDeleter>(
            glfwCreateWindow(m_size.x(), m_size.y(), m_title.c_str(), nullptr, nullptr));

        glfwMakeContextCurrent(m_handle.get());

        this->set_callbacks();

        if (gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)) == 0) {
            fmt::print(stderr, "[GLAD/ERROR] Failed to initialize\n");
            glfwTerminate();
            std::exit(1);
        }

        glfwSwapInterval(1);
    }

    auto Window::main_loop() -> void
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
                this->tick();

                tick_time -= NS_PER_TICK;
            }

            m_tick_remainder = std::max(tick_time, 0ULL);

            this->update();
            this->render();

            glfwSwapBuffers(m_handle.get());
            glfwPollEvents();
        }
    }

    Window::Window() : m_size(details::DEFAULT_SIZE)
    {
        auto now = util::time::get_time_ns();

        m_last_frame  = now;
        m_last_second = now;

        WindowFlags hints;
        this->init_glfw(hints);
        this->main_loop();
    }

    Window::Window(Vec2<i32> const &size) : m_size(size)
    {
        auto now = util::time::get_time_ns();

        m_last_frame  = now;
        m_last_second = now;

        WindowFlags hints;
        this->init_glfw(hints);
        this->main_loop();
    }

    auto Window::get_size() const -> Vec2<i32>
    {
        return (m_size);
    }
}  // namespace platform
