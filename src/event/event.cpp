/** @file event.cpp */

#include "event.hpp"

namespace event
{
    Event::Event()
    {
        m_type     = EventType::None;
        m_category = EventCategory::None;
    }

    Event::Event(EventType const &type)
    {
        m_type     = type;
        m_category = this->get_category_from_type(type);
        m_name     = this->get_name_from_type(type);
    }

    auto Event::get_name_from_type(EventType const &type) const -> char const *
    {
        auto result = "";

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
            case EventType::None:                   { result = EventCategory::None;       }; break;
            case EventType::WindowClose:            { result = EventCategory::Window;     }; break;
            case EventType::WindowResize:           { result = EventCategory::Window;     }; break;
            case EventType::WindowFocus:            { result = EventCategory::Window;     }; break;
            case EventType::WindowLostFocus:        { result = EventCategory::Window;     }; break;
            case EventType::WindowMoved:            { result = EventCategory::Window;     }; break;
            case EventType::LoadResource:           { result = EventCategory::Resource;   }; break;
            case EventType::FreeResource:           { result = EventCategory::Resource;   }; break;
            case EventType::EngineTick:             { result = EventCategory::Engine;     }; break;
            case EventType::EngineUpdate:           { result = EventCategory::Engine;     }; break;
            case EventType::EngineRender:           { result = EventCategory::Engine;     }; break;
            case EventType::KeyPressed:             { result = EventCategory::Input;      }; break;
            case EventType::KeyReleased:            { result = EventCategory::Input;      }; break;
            case EventType::MouseButtonPressed:     { result = EventCategory::Input;      }; break;
            case EventType::MouseButtonReleased:    { result = EventCategory::Input;      }; break;
            case EventType::MouseMoved:             { result = EventCategory::Input;      }; break;
            case EventType::MouseScrolled:          { result = EventCategory::Input;      }; break;
            default:                                { result = EventCategory::Custom;     }; break;
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

    auto Event::to_string() const -> std::string
    {
        if (!m_name.empty()) {
            return (m_name);
        } else {
            return (this->get_name_from_type(m_type));
        }
    }
}  // namespace event
