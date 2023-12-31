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
    namespace detail
    {
        /** Default window size */
        Vec2<i32> constexpr const DEFAULT_SIZE { 800, 600 };

    }  // namespace detail

    /***************************************************************************************************
     * Describes a window and all its properties
     **************************************************************************************************/
    class Window
    {
    protected:
        // ========================================================================================== //
        // Associated data structures =============================================================== //
        // ========================================================================================== //

        /***********************************************************************************************
         * Sets the properties and flags for `GLFW3`
         **********************************************************************************************/
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
        class Emitter: public event::Emitter<Window::Emitter>
        {
        public:
            auto emit(event::Event const &event) -> void;
        };

        // ========================================================================================== //
        // Implementation methods =================================================================== //
        // ========================================================================================== //

        /** Called each tick */
        auto virtual tick() -> void = 0;

        /** Called each update */
        auto virtual update() -> void = 0;

        /** Called on each frame */
        auto virtual render() -> void = 0;

    public:
        // ========================================================================================== //
        // Constructors and destructors ============================================================= //
        // ========================================================================================== //

        Window();
        explicit Window(Vec2<i32> const &size);
        virtual ~Window() = 0;

        // ========================================================================================== //
        // Accessor methods ========================================================================= //
        // ========================================================================================== //

        [[nodiscard]] auto virtual get_size() const -> Vec2<i32> = 0;

        // ========================================================================================== //
        // Utility methods ========================================================================== //
        // ========================================================================================== //

        auto virtual set_platform_data(bgfx::PlatformData &platform_data) -> void = 0;
        auto virtual prepare_frame() -> void                                      = 0;
        auto virtual end_frame() -> void                                          = 0;
        auto virtual is_close_requested() -> bool                                 = 0;
        auto virtual close() -> void                                              = 0;
    };
}  // namespace platform
