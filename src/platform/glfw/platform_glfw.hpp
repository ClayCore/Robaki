/** @file platform_glfw.hpp */

#pragma once

// module includes
#include "platform/input.hpp"
#include "platform/window.hpp"
#include "util/util.hpp"


namespace platform::glfw
{
    namespace detail
    {
        struct WindowDeleter
        {
            inline auto operator()(GLFWwindow *handle) -> void
            {
                glfwDestroyWindow(handle);
                glfwTerminate();
            }
        };
    }  // namespace detail

    struct Keyboard;
    struct Mouse;

    class Window : public platform::Window
    {
    private:
        // ========================================================================================== //
        // Properties and data ====================================================================== //
        // ========================================================================================== //

        /** Window size */
        Vec2<i32> m_size = {};

        /** Window title */
        std::string m_title = { "Worming v0.0.1" };

        /** Handle to internal window handle */
        std::unique_ptr<GLFWwindow, detail::WindowDeleter> m_handle;

        /** Handle to mouse */
        Scope<Mouse> m_mouse;

        /** Handle to keyboard */
        Scope<Keyboard> m_keyboard;

        /** Internal emitter for firing events */
        platform::Window::Emitter m_emitter;

        // ========================================================================================== //
        // Timing variables ========================================================================= //
        // ========================================================================================== //

        u64 m_fps         = { 0 };
        u64 m_frame_delta = { 0 };
        u64 m_frames      = { 0 };
        u64 m_last_frame  = { 0 };

        u64 m_tick_remainder = { 0 };
        u64 m_last_second    = { 0 };
        u64 m_ticks          = { 0 };
        u64 m_tps            = { 0 };

        // ========================================================================================== //
        // Utility methods ========================================================================== //
        // ========================================================================================== //

        /**
         * Sets `GLFW` window flags and properties
         *
         * @param flags flags to set
         */
        static auto set_flags(WindowFlags flags) -> void;

        /** Called on each tick */
        auto tick() -> void override;

        /** Called on each update */
        auto update() -> void override;

        /** Called on each frame */
        auto render() -> void override;

        /**
         * Initialize `GLFW` and all window flags
         *
         * @param flags flags to be set
         */
        auto init_glfw(WindowFlags flags, Vec2<i32> const &size) -> void;

        /** Run the main window loop */
        auto main_loop() -> void;

    public:
        // ========================================================================================== //
        // Constructors and destructors ============================================================= //
        // ========================================================================================== //

        Window();

        Window(const Window &)            = delete;
        Window(Window &&)                 = delete;
        Window &operator=(const Window &) = delete;
        Window &operator=(Window &&)      = delete;

        explicit Window(Vec2<i32> const &size);
        ~Window() override;

        // ========================================================================================== //
        // Accessor methods ========================================================================= //
        // ========================================================================================== //

        /**
         * Get the current size of the window
         *
         * @return size of window
         */
        [[nodiscard]] auto get_size() const -> Vec2<i32> override;

        // ========================================================================================== //
        // Utility methods ========================================================================== //
        // ========================================================================================== //

        auto set_platform_data(bgfx::PlatformData &platform_data) -> void override;

        auto prepare_frame() -> void override;

        auto end_frame() -> void override;

        auto is_close_requested() -> bool override;

        auto close() -> void override;
    };

    struct Keyboard final : platform::Keyboard
    {
    private:
        auto callback(GLFWwindow *window, i32 key, i32 scancode, i32 action, i32 mods) -> void;

    public:
        static const constexpr usize MAX_KEYS = 1024;
        std::unordered_map<std::string, Button> m_keys;

        explicit Keyboard(platform::glfw::Window &window);
        auto get_buttons() -> util::Iterator<platform::Button *> override;
        auto operator[](std::string const &name) -> std::optional<Button *> override;
    };

    struct Mouse final : platform::Mouse
    {
    private:
        auto callback_pos(GLFWwindow *window, f64 xpos, f64 ypos) -> void;
        auto callback_button(GLFWwindow *window, i32 button, i32 action, i32 mods) -> void;
        auto callback_enter(GLFWwindow *window, i32 entered) -> void;
        auto callback_scroll(GLFWwindow *window, f64 xpos, f64 ypos) -> void;

    public:
        Window *window;
        explicit Mouse(platform::glfw::Window &window);

        auto set_mode(platform::Mouse::Mode mode) -> void override;
    };
}  // namespace platform::glfw
