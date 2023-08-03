/** @file handler.hpp */

#pragma once

// module includes
#include "event.hpp"
#include "util/util.hpp"

// c++ includes
#include <unordered_map>

namespace event
{
    // Forward reference to subscriber...
    class Subscriber;

    /**
     * Describes a base provider class
     * Each and every handler (`Emitter`, `Dispatcher`, `Listener`) inherits this structure
     *
     * @tparam Derived specific implementation of `Provider`
     */
    template <typename Derived>
    class Provider
    {
    protected:
        /** Utility type alias for callbacks */
        using Callback = void (*)();

        /** Contains all dispatchers bound to the provider */
        std::vector<Ref<Provider>> m_dispatchers;

        /** Contains all subscribers bound to the provider */
        std::vector<Ref<Subscriber>> m_subscribers;

        /** Maps each event to a list of callbacks that are meant to be fired on that event */
        std::unordered_map<Event, std::vector<Callback>, EventHasher, EventEqualizer> m_actions;

        /**
         * Provides mutable access to the concrete derived type
         *
         * @return reference to concrete derived type
         */
        inline auto derived() -> Derived &
        {
            return (static_cast<Derived &>(*this));
        }

        /**
         * Provides read-only access to the concrete derived type
         *
         * @return const reference to concrete derived type
         */
        inline auto derived() const -> Derived const &
        {
            return (static_cast<Derived const &>(*this));
        }

    public:
        // ================================================================================================ //
        // Constructors and destructors =================================================================== //
        // ================================================================================================ //

        Provider()                            = default;
        ~Provider()                           = default;
        Provider(const Provider &)            = default;
        Provider(Provider &&)                 = delete;
        Provider &operator=(const Provider &) = default;
        Provider &operator=(Provider &&)      = delete;

        // ================================================================================================ //
        // Accessor methods =============================================================================== //
        // ================================================================================================ //

        /**
         * Returns the specific dispatcher
         *
         * @param index index of the dispatcher
         * @return pointer to the dispatcher
         */
        [[nodiscard]] auto get_dispatcher(usize index) -> Ref<Provider>
        {
            return (m_dispatchers[index]);
        }

        /**
         * Returns the specific subscriber
         *
         * @param index index of the subscriber
         * @return pointer to the subscriber
         */
        [[nodiscard]] auto get_subscriber(usize index) -> Ref<Subscriber>
        {
            return (m_subscribers[index]);
        }

        /**
         * Returns the specific callback mapped to the event
         *
         * @param event event to be found
         * @return callbacks mapped to the event
         */
        [[nodiscard]] auto get_callbacks(Event const &event) -> std::vector<Callback> &
        {
            return (m_actions.at(event));
        }

        // ================================================================================================ //
        // Mutator methods ================================================================================ //
        // ================================================================================================ //

        /**
         * Adds a dispatcher
         *
         * @param dispatcher dispatcher to add
         */
        auto add_dispatcher(Ref<Provider> dispatcher) -> void
        {
            m_dispatchers.push_back(dispatcher);
        }

        /**
         * Adds a subscriber
         *
         * @param obj subscriber to be added
         */
        auto add_subscriber(Ref<Subscriber> const &subscriber) -> void
        {
            m_subscribers.push_back(subscriber);
        }

        /**
         * Adds a callback to the mapped event
         *
         * @param event key for the callback
         * @param callback callback to be mapped in the list
         */
        auto add_callback(Event const &event, Callback callback) -> void
        {
            m_actions[event].push_back(callback);
        }

        // ================================================================================================ //
        // Specific provider implementation =============================================================== //
        // ================================================================================================ //

        /**
         * Forwards the event to a listener
         *
         * @param event event to forward
         */
        auto dispatch(Event const &event) -> void
        {
            this->derived().dispatch(event);
        }

        /**
         * Fires the event to subscribed dispatchers
         *
         * @param event event to fire
         */
        auto emit(Event const &event) -> void
        {
            this->derived().emit(event);
        }

        /**
         * Triggers a callback for the event
         *
         * @param event subscribed event
         */
        auto listen(Event const &event) -> void
        {
            this->derived().listen(event);
        }
    };
}  // namespace event
