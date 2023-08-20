/** @file input.hpp */

#pragma once

// module includes
#include "event/listener.hpp"
#include "event/subscriber.hpp"
#include "platform/window.hpp"
#include "util/util.hpp"
#include "window.hpp"

// c++ includes
#include <optional>
#include <utility>


namespace platform
{
    /***************************************************************************************************
     * Describes a single button and provides an event listener
     * via the inherited `Subscriber` class
     **************************************************************************************************/
    struct Button: public event::Subscriber
    {
        // ========================================================================================== //
        // Associated data ========================================================================== //
        // ========================================================================================== //

        /** ID of the button (used by GLFW3) */
        usize id = { 0 };

        /** Refer to any button via name (also thanks to GLFW3) */
        std::string name = { "None" };

        /** Current button state */
        bool down = { false };

        /** Last button state */
        bool last = { false };

        /** State of button on last tick */
        bool last_tick = { false };

        /** Is the button still pressed? */
        bool pressed = { false };

        /** Last tick when the button was still pressed */
        bool pressed_tick = { false };

        // ========================================================================================== //
        // Constructors and destructors ============================================================= //
        // ========================================================================================== //

        Button() = default;
        Button(usize p_id, std::string p_name): id(p_id), name(std::move(p_name))
        {
        }

        // ========================================================================================== //
        // Button implementation methods ============================================================ //
        // ========================================================================================== //

        /***********************************************************************************************
         * Toggle the button if it's not pressed anymore
         *
         * Called on each update
         **********************************************************************************************/
        constexpr auto update() -> void
        {
            pressed = down && !last;
            last    = down;
        }

        /***********************************************************************************************
         * Toggle the button if it's not pressed anymore
         *
         * Called on each tick
         **********************************************************************************************/
        constexpr auto tick() -> void
        {
            pressed_tick = down && !last_tick;
            last_tick    = down;
        }
    };

    /***************************************************************************************************
     * Describes any generic input device
     *
     * Inheritable for custom input devices.
     **************************************************************************************************/
    struct Input
    {
        // ========================================================================================== //
        // Constructors and destructors ============================================================= //
        // ========================================================================================== //
        Input()              = default;
        Input(Input const &) = default;
        Input(Input &&)      = delete;

        auto operator=(Input const &) -> Input & = default;
        auto operator=(Input &&) -> Input      & = delete;

        virtual ~Input() = default;

        // ========================================================================================== //
        // Input implementation methods (for overriding) ============================================ //
        // ========================================================================================== //

        /***********************************************************************************************
         * Update all the buttons for the device, called on each update
         **********************************************************************************************/
        virtual auto update() -> void;

        /***********************************************************************************************
         * Tick all the buttons for the device, called on each tick
         **********************************************************************************************/
        virtual auto tick() -> void;

        /***********************************************************************************************
         * Acquire an iterator to all `Button` derivatives.
         *
         * @return iterator
         **********************************************************************************************/
        virtual auto get_buttons() -> util::Iterator<Button *>;
    };

    /** Describes a basic keyboard input device */
    struct Keyboard: Input
    {
        virtual auto operator[](std::string const &name) -> std::optional<Button *> = 0;
    };

    /** Describes a basic mouse input device */
    struct Mouse: Input
    {
    private:
        /** Last recorded position */
        Vec2<f32> last {};

        /** Last ticked position */
        Vec2<f32> last_tick {};

        /** Last scrolled position */
        f32 last_scroll { 0.0F };

        /** Last ticked scroll position */
        f32 last_scroll_tick { 0.0F };

    public:
        /** Describes the mode in which the mouse operates */
        enum Mode : u8
        {
            Disabled,
            Hidden,
            Normal
        };

        /** Used for indexing the mouse buttons */
        enum Index : u8
        {
            Left   = 0,
            Right  = 1,
            Middle = 2,
        };

        Mode mode                   = { Disabled };
        std::vector<Button> buttons = {
            { Index::Left, "left" },
            { Index::Right, "right" },
            { Index::Middle, "middle" },
        };

        /** Current position */
        Vec2<f32> pos {};

        /** Current delta position */
        Vec2<f32> pos_delta {};

        /** Normalized position */
        Vec2<f32> pos_norm {};

        /** Normalized delta position */
        Vec2<f32> pos_delta_norm {};

        /** Delta position on last tick */
        Vec2<f32> pos_delta_tick {};

        /** Normalized delta position on last tick */
        Vec2<f32> pos_delta_norm_tick {};

        /** Current scroll value */
        f32 scroll { 0.0F };

        /** Current scroll delta */
        f32 scroll_delta { 0.0F };

        /** Scroll delta on last tick */
        f32 scroll_delta_tick { 0.0F };

        /** Whether the mouse is contained in a window */
        bool in_window;

        virtual auto set_mode(Mode mode) -> void = 0;

        Mouse() = default;

        auto get_buttons() -> util::Iterator<Button *> override;
        auto update() -> void override;
        auto tick() -> void override;

        virtual auto operator[](std::string const &name) -> std::optional<Button *>;
    };
}  // namespace platform
