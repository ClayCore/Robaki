/** @file input.cpp */

#include "input.hpp"
#include "managers/state_manager.hpp"

namespace platform
{
    auto Input::update() -> void
    {
        for (auto &button : this->get_buttons()) {
            button->update();
        }
    }

    auto Input::tick() -> void
    {
        for (auto &button : this->get_buttons()) {
            button->tick();
        }
    }

    auto Input::get_buttons() -> util::Iterator<Button *>
    {
        return (util::Iterator<Button *> {});
    }

    auto Mouse::get_buttons() -> util::Iterator<Button *>
    {
        return (util::iter(buttons).ptr());
    }

    auto Mouse::update() -> void
    {
        auto size = managers::StateManager::instance().get_window()->get_size();

        Input::update();

        pos_delta      = pos - last;
        pos_delta_norm = pos_delta / size;

        last = pos;

        scroll_delta = scroll - last_scroll;
        last_scroll  = scroll;

        scroll_delta = scroll - last_scroll;
        last_scroll  = scroll;
    }

    auto Mouse::tick() -> void
    {
        auto size = managers::StateManager::instance().get_window()->get_size();

        Input::tick();

        pos_delta_tick      = pos - last_tick;
        pos_delta_norm_tick = pos_delta_tick / size;

        last_tick = pos;

        scroll_delta_tick = scroll - last_scroll_tick;
        last_scroll_tick  = scroll;
    }

    auto Mouse::operator[](std::string const &name) -> std::optional<Button *>
    {
        auto name_lowered = util::string::to_lower(name);

        if (name_lowered == "left") {
            return (std::make_optional(&this->buttons[Index::Left]));
        } else if (name_lowered == "right") {
            return (std::make_optional(&this->buttons[Index::Right]));
        } else if (name_lowered == "middle") {
            return (std::make_optional(&this->buttons[Index::Middle]));
        }

        return (std::nullopt);
    }
}  // namespace platform
