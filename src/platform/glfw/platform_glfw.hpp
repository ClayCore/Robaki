/** @file platform_glfw.hpp */

#pragma once

// module includes
#include "platform/input.hpp"
#include "platform/window.hpp"
#include "util/util.hpp"


namespace platform::glfw
{
    namespace details
    {
        struct WindowDeleter
        {
            inline auto operator()(GLFWwindow *handle) -> void
            {
                glfwDestroyWindow(handle);
                glfwTerminate();
            }
        };
    }  // namespace details

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
        std::unique_ptr<GLFWwindow, details::WindowDeleter> m_handle;

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
        auto init_glfw(WindowFlags flags) -> void;

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
    };
}  // namespace platform::glfw
