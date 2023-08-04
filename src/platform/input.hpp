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
    struct Button : public event::Subscriber
    {
        usize id;
        std::string name;

        bool down;
        bool last, last_tick;
        bool pressed, pressed_tick;

        Button() = default;
        Button(usize id, std::string name) : id(id), name(std::move(name))
        {
        }

        constexpr auto update() -> void
        {
            pressed = down && !last;
            last    = down;
        }

        constexpr auto tick() -> void
        {
            pressed_tick = down && !last_tick;
            last_tick    = down;
        }
    };

    struct Input
    {
        Input(const Input &)            = default;
        Input(Input &&)                 = delete;
        Input &operator=(const Input &) = default;
        Input &operator=(Input &&)      = delete;
        virtual ~Input()                = default;

        virtual auto update() -> void
        {
            for (auto &button : this->get_buttons()) {
                button->update();
            }
        }

        virtual auto tick() -> void
        {
            for (auto &button : this->get_buttons()) {
                button->tick();
            }
        }

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
        f32 xpos_last = { 0 };
        f32 ypos_last = { 0 };

        f32 xpos_last_tick = { 0 };
        f32 ypos_last_tick = { 0 };

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

        f32 xpos            = { 0.0F };
        f32 ypos            = { 0.0F };
        f32 xpos_normalized = { 0.0F };
        f32 ypos_normalized = { 0.0F };

        f32 xpos_delta            = { 0.0F };
        f32 ypos_delta            = { 0.0F };
        f32 xpos_delta_normalized = { 0.0F };
        f32 ypos_delta_normalized = { 0.0F };

        f32 xpos_delta_tick            = { 0.0F };
        f32 ypos_delta_tick            = { 0.0F };
        f32 xpos_delta_tick_normalized = { 0.0F };
        f32 ypos_delta_tick_normalized = { 0.0F };

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

            xpos_delta            = xpos - xpos_last;
            xpos_delta_normalized = xpos_delta / 800;

            ypos_delta            = ypos - ypos_last;
            ypos_delta_normalized = ypos_delta / 600;

            xpos_last = xpos;
            ypos_last = ypos;

            scroll_delta = scroll - last_scroll;
            last_scroll  = scroll;
        }

        auto tick() -> void override
        {
            Input::tick();

            xpos_delta_tick            = xpos - xpos_last_tick;
            xpos_delta_tick_normalized = xpos_delta_tick / 800;

            ypos_delta_tick            = ypos - ypos_last_tick;
            ypos_delta_tick_normalized = ypos_delta_tick / 600;

            xpos_last_tick = xpos;
            ypos_last_tick = ypos;

            scroll_delta_tick = scroll - last_scroll_tick;
            last_scroll_tick  = scroll;
        }


        // virtual auto operator[](std::string const &name) -> std::optional<Button *>
        // {
        //     return std::nullopt_t;
        // }
    };
}  // namespace platform
