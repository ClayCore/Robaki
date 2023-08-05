/** @file input.hpp */

#pragma once

// module includes
#include "event/listener.hpp"
#include "event/subscriber.hpp"
#include "util/util.hpp"

// c++ includes
#include <optional>
#include <utility>


namespace platform
{
    /**
     * Describes a single button and provides an event listener
     * via the inherited `Subscriber` class
     */
    struct Button : public event::Subscriber
    {
        // ================================================================================================ //
        // Associated data ================================================================================ //
        // ================================================================================================ //

        /** ID of the button (used by GLFW3) */
        usize id = { 0 };

        /** Refer to any button via name (also thanks to GLFW3) */
        std::string name = { "None" };

        bool down         = { false };
        bool last         = { false };
        bool last_tick    = { false };
        bool pressed      = { false };
        bool pressed_tick = { false };

        // ================================================================================================ //
        // Constructors and destructors =================================================================== //
        // ================================================================================================ //

        Button() = default;
        Button(usize p_id, std::string p_name) : id(p_id), name(std::move(p_name))
        {
        }

        // ================================================================================================ //
        // Button implementation methods ================================================================== //
        // ================================================================================================ //

        /**
         * Toggle the button if it's not pressed anymore
         *
         * Called on each update
         */
        constexpr auto update() -> void
        {
            pressed = down && !last;
            last    = down;
        }

        /**
         * Toggle the button if it's not pressed anymore
         *
         * Called on each tick
         */
        constexpr auto tick() -> void
        {
            pressed_tick = down && !last_tick;
            last_tick    = down;
        }
    };

    /**
     * Describes any generic input device
     *
     * Inheritable for custom input devices.
     */
    struct Input
    {
        // ================================================================================================ //
        // Constructors and destructors =================================================================== //
        // ================================================================================================ //
        Input(const Input &)            = default;
        Input(Input &&)                 = delete;
        Input &operator=(const Input &) = default;
        Input &operator=(Input &&)      = delete;
        virtual ~Input()                = default;

        // ================================================================================================ //
        // Input implementation methods (for overriding) ================================================== //
        // ================================================================================================ //

        /**
         * Update all the buttons for the device, called on each update
         */
        virtual auto update() -> void
        {
            for (auto &button : this->get_buttons()) {
                button->update();
            }
        }

        /**
         * Tick all the buttons for the device, called on each tick
         */
        virtual auto tick() -> void
        {
            for (auto &button : this->get_buttons()) {
                button->tick();
            }
        }

        /**
         * Acquire an iterator to all `Button` derivatives.
         *
         * @return iterator
         */
        virtual auto get_buttons() -> util::Iterator<Button *>
        {
            return (util::Iterator<Button *>{});
        }
    };

    struct Keyboard : Input
    {
        virtual auto operator[](std::string const &name) -> std::optional<Button *> = 0;
    };

    struct Mouse : Input
    {
    private:
        Vec2<f32> last;

        Vec2<f32> last_tick;

        f32 last_scroll      = { 0 };
        f32 last_scroll_tick = { 0 };

    public:
        enum class Mode
        {
            Disabled,
            Hidden,
            Normal
        };

        enum class Index
        {
            Left   = 0,
            Right  = 1,
            Middle = 2,
        };

        Mode mode;
        std::vector<Button> buttons;

        Vec2<f32> pos;
        Vec2<f32> pos_delta;
        Vec2<f32> pos_norm;
        Vec2<f32> pos_delta_norm;

        Vec2<f32> pos_delta_tick;
        Vec2<f32> pos_delta_norm_tick;

        f32 scroll            = { 0 };
        f32 scroll_delta      = { 0 };
        f32 scroll_delta_tick = { 0 };

        bool in_window;

        virtual auto set_mode(Mode mode) -> void = 0;

        Mouse();

        auto get_buttons() -> util::Iterator<Button *> override
        {
            return (util::iter(buttons).ptr());
        }

        auto update() -> void override
        {
            Input::update();

            pos_delta      = pos - last;
            pos_delta_norm = pos_delta / Vec2<i32>{ 800, 600 };

            last = pos;

            scroll_delta = scroll - last_scroll;
            last_scroll  = scroll;

            scroll_delta = scroll - last_scroll;
            last_scroll  = scroll;
        }

        auto tick() -> void override
        {
            Input::tick();

            pos_delta_tick      = pos - last_tick;
            pos_delta_norm_tick = pos_delta_tick / Vec2<i32>{ 800, 600 };

            last_tick = pos;

            scroll_delta_tick = scroll - last_scroll_tick;
            last_scroll_tick  = scroll;
        }


        // virtual auto operator[](std::string const &name) -> std::optional<Button *>
        // {
        //     return std::nullopt_t;
        // }
    };
}  // namespace platform
