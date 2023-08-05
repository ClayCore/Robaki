/** @file window.hpp */

#pragma once

// module includes
#include "event/emitter.hpp"
#include "input.hpp"
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
        /** Default window size */
        constexpr const Vec2<i32> DEFAULT_SIZE{ 800, 600 };

        /** Used by `Window` to create a unique pointer with window deletion support */
        struct WindowDeleter
        {
            inline auto operator()(GLFWwindow *window) -> void
            {
                glfwDestroyWindow(window);
                glfwTerminate();
            }
        };
    }  // namespace details

    /**
     * Describes a window and all its properties
     */
    class Window
    {
    private:
        // ========================================================================================== //
        // Associated data structures =============================================================== //
        // ========================================================================================== //

        /**
         * Sets the properties and flags for `GLFW3`
         */
        struct WindowFlags
        {
            /** Is a window resizeable? */
            i32 resizeable = { GL_FALSE };

            /** OpenGL major version support*/
            i32 version_major = { 4 };

            /** OpenGL minor version support */
            i32 version_minor = { 1 };

            /** OpenGL profile */
            i32 profile = { GLFW_OPENGL_CORE_PROFILE };

            /** OpenGL forward compatibility check */
            i32 forward_compat = { GL_TRUE };
        };

        /** Allows the window to emit events to the `StateManager` */
        class Emitter : public event::Emitter<Window::Emitter>
        {
        public:
            auto emit(event::Event const &event) -> void;
        };

        // ========================================================================================== //
        // Properties and data ====================================================================== //
        // ========================================================================================== //

        /** Size of the window */
        Vec2<i32> m_size = {};

        /** Window title */
        std::string m_title = { "Worming v0.0.1" };

        /** Handle to the underlying `GLFWwindow` instance */
        std::unique_ptr<GLFWwindow, details::WindowDeleter> m_handle;

        /** Instance of the internal emitter */
        Emitter m_emitter;

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

        /**
         * Sets `GLFW` window callbacks for events
         *
         * @param flags flags to set
         */
        auto set_callbacks() -> void;

        /** Called each tick */
        auto tick() -> void;

        /** Called each update */
        auto update() -> void;

        /** Called on each frame */
        auto render() -> void;

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
        // Constructors ============================================================================= //
        // ========================================================================================== //

        Window();
        explicit Window(Vec2<i32> const &size);

        [[nodiscard]] auto get_size() const -> Vec2<i32>;
    };
}  // namespace platform
