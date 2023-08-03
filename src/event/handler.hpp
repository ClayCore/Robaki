/** @file handler.hpp */

#pragma once

// module includes
#include "event.hpp"
#include "object.hpp"
#include "util/util.hpp"


namespace event
{
    template <typename Derived>
    class Handler
    {
    protected:
        using Callback = void (*)();

        std::vector<Ref<Handler>> m_dispatchers;
        std::vector<Ref<Object>> m_objects;
        std::unordered_map<Event, std::vector<Callback>, EventHasher, EventEqualizer> m_actions;

        inline auto derived() -> Derived &
        {
            return (static_cast<Derived &>(*this));
        }

        inline auto derived() const -> Derived const &
        {
            return (static_cast<Derived const &>(*this));
        }

    public:
        ~Handler() = default;

        [[nodiscard]] virtual auto get_dispatcher(usize index) -> Ref<Handler>
        {
            return (m_dispatchers[index]);
        }

        [[nodiscard]] virtual auto get_object(usize index) -> Ref<Object>
        {
            return (m_objects[index]);
        }

        [[nodiscard]] virtual auto get_callbacks(Event const &event) -> std::vector<Callback> &
        {
            return (m_actions.at(event));
        }

        auto add_dispatcher(Ref<Handler> dispatcher) -> void
        {
            m_dispatchers.push_back(dispatcher);
        }

        auto add_object(Ref<Object> const &obj) -> void
        {
            m_objects.push_back(obj);
        }

        auto add_callback(Event const &event, Callback callback) -> void
        {
            m_actions[event].push_back(callback);
        }

        auto dispatch(Event const &event) -> void
        {
            this->derived().dispatch(event);
        }

        auto emit(Event const &event) -> void
        {
            this->derived().emit(event);
        }

        auto listen(Event const &event) -> void
        {
            this->derived().listen(event);
        }
    };
}  // namespace event
