/** @file event.cpp */

#include "event.hpp"

namespace event
{
    Event::Event(EventType const &type)
      : m_type(type), m_category(this->get_category_from_type(type)), m_name(this->get_name_from_type(type))
    {
    }

    auto Event::get_name_from_type(EventType const &type) const -> char const *
    {
        auto const *result = "";

        // clang-format off
        switch (type) {
            case EventType::None:                   { result = "None";                }; break;
            case EventType::WindowClose:            { result = "WindowClose";         }; break;
            case EventType::WindowResize:           { result = "WindowResize";        }; break;
            case EventType::WindowFocus:            { result = "WindowFocus";         }; break;
            case EventType::WindowLostFocus:        { result = "WindowLostFocus";     }; break;
            case EventType::WindowMoved:            { result = "WindowMoved";         }; break;
            case EventType::LoadResource:           { result = "LoadResource";        }; break;
            case EventType::FreeResource:           { result = "FreeResource";        }; break;
            case EventType::EngineTick:             { result = "EngineTick";          }; break;
            case EventType::EngineUpdate:           { result = "EngineUpdate";        }; break;
            case EventType::EngineRender:           { result = "EngineRender";        }; break;
            case EventType::KeyPressed:             { result = "KeyPressed";          }; break;
            case EventType::KeyReleased:            { result = "KeyReleased";         }; break;
            case EventType::MouseButtonPressed:     { result = "MouseButtonPressed";  }; break;
            case EventType::MouseButtonReleased:    { result = "MouseButtonReleased"; }; break;
            case EventType::MouseMoved:             { result = "MouseMoved";          }; break;
            case EventType::MouseScrolled:          { result = "MouseScrolled";       }; break;
            default:                                { result = "Unimplemented";       }; break;
        }
        // clang-format on

        return (result);
    }

    auto Event::get_category_from_type(EventType const &type) const -> EventCategory
    {
        auto result = EventCategory::None;

        // clang-format off
        switch (type) {
            case EventType::None:                { result = EventCategory::None;       }; break;
            case EventType::WindowClose:
            case EventType::WindowResize:
            case EventType::WindowFocus:
            case EventType::WindowLostFocus:
            case EventType::WindowMoved:         { result = EventCategory::Window;     }; break;
            case EventType::LoadResource:
            case EventType::FreeResource:        { result = EventCategory::Resource;   }; break;
            case EventType::EngineTick:
            case EventType::EngineUpdate:
            case EventType::EngineRender:        { result = EventCategory::Engine;     }; break;
            case EventType::KeyPressed:
            case EventType::KeyReleased:
            case EventType::MouseButtonPressed:
            case EventType::MouseButtonReleased:
            case EventType::MouseMoved:
            case EventType::MouseScrolled:       { result = EventCategory::Input;      }; break;
            default:                             { result = EventCategory::Custom;     }; break;
        }
        // clang-format on

        return (result);
    }

    auto Event::set_type(EventType const &type) -> void
    {
        m_type = type;
    }

    auto Event::set_name(std::string const &name) -> void
    {
        m_name = name;
    }

    auto Event::set_category(EventCategory const &category) -> void
    {
        m_category = category;
    }

    auto Event::to_string() const -> std::string
    {
        if (!m_name.empty()) {
            return (m_name);
        } else {
            return (this->get_name_from_type(m_type));
        }
    }

    auto Event::category_to_string(EventCategory const &category) const -> char const *
    {
        auto const *result = "";

        // clang-format off
        switch (category) {
            case EventCategory::None:       { result = "EventCategory::None";      }; break;
            case EventCategory::Window:     { result = "EventCategory::Window";    }; break;
            case EventCategory::Resource:   { result = "EventCategory::Resource";  }; break;
            case EventCategory::Engine:     { result = "EventCategory::Engine";    }; break;
            case EventCategory::Input:      { result = "EventCategory::Input";     }; break;
            default:                        { result = "EventCategory::Custom";    }; break;
        }
        // clang-format on

        return (result);
    }

    auto EventHasher::operator()(Event const &event) const -> usize
    {
        auto const *name            = event.get_name();
        auto category               = event.get_category();
        auto const *category_string = event.category_to_string(category);

        auto name_hash     = std::hash<std::string>{}(name);
        auto category_hash = std::hash<std::string>{}(category_string);

        return (name_hash ^ (category_hash << 1U));
    }

    auto EventEqualizer::operator()(Event const &lhs, Event const &rhs) const -> bool
    {
        return (lhs == rhs);
    }
}  // namespace event
