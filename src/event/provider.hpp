/** @file provider.hpp */

#pragma once

// module includes
#include "event.hpp"
#include "util/traits.hpp"
#include "util/util.hpp"

// c++ includes
#include <unordered_map>

namespace event
{
    // Forward reference to subscriber...
    class Subscriber;

    /***************************************************************************************************
     * Describes a base provider class
     * Each and every handler (`Emitter`, `Dispatcher`, `Listener`) inherits this structure
     *
     * @tparam Derived specific `Provider` implementation
     **************************************************************************************************/
    template <class Derived>
    class Provider
    {
    private:
        /***********************************************************************************************
         * Constraints the `Derived` type
         **********************************************************************************************/
        auto constexpr check_static() -> void
        {
            static_assert(Emittable<Derived> || Dispatchable<Derived> || Listenable<Derived>);
        }

    protected:
        /** Utility type alias for callbacks */
        using Callback = void (*)();

        /** Contains all dispatchers bound to the provider */
        std::vector<Ref<Provider>> m_dispatchers;

        /** Contains all subscribers bound to the provider */
        std::vector<Ref<Subscriber>> m_subscribers;

        /** Maps each event to a list of callbacks that are meant to be fired on that event */
        std::unordered_map<Event, std::vector<Callback>, EventHasher, EventEqualizer> m_actions;

        /***********************************************************************************************
         * Provides mutable access to the concrete derived type
         *
         * @return reference to concrete derived type
         **********************************************************************************************/
        auto inline derived() -> Derived &
        {
            return (static_cast<Derived &>(*this));
        }

        /***********************************************************************************************
         * Provides read-only access to the concrete derived type
         *
         * @return const reference to concrete derived type
         **********************************************************************************************/
        auto inline derived() const -> Derived const &
        {
            return (static_cast<Derived const &>(*this));
        }

    public:
        // ========================================================================================== //
        // Constructors and destructors ============================================================= //
        // ========================================================================================== //

        Provider()  = default;
        ~Provider() = default;

        Provider(Provider const &) = default;
        Provider(Provider &&)      = delete;

        auto operator=(Provider const &) -> Provider & = default;
        auto operator=(Provider &&) -> Provider      & = delete;

        // ========================================================================================== //
        // Accessor methods ========================================================================= //
        // ========================================================================================== //

        /***********************************************************************************************
         * Returns the specific dispatcher
         *
         * @param index index of the dispatcher
         * @return pointer to the dispatcher
         **********************************************************************************************/
        [[nodiscard]] auto get_dispatcher(usize index) -> Ref<Provider>
        {
            return (m_dispatchers[index]);
        }

        /***********************************************************************************************
         * Returns the specific subscriber
         *
         * @param index index of the subscriber
         * @return pointer to the subscriber
         **********************************************************************************************/
        [[nodiscard]] auto get_subscriber(usize index) -> Ref<Subscriber>
        {
            return (m_subscribers[index]);
        }

        /***********************************************************************************************
         * Returns the specific callback mapped to the event
         *
         * @param event event to be found
         * @return callbacks mapped to the event
         **********************************************************************************************/
        [[nodiscard]] auto get_callbacks(Event const &event) -> std::vector<Callback> &
        {
            return (m_actions.at(event));
        }

        // ========================================================================================== //
        // Mutator methods ========================================================================== //
        // ========================================================================================== //

        /***********************************************************************************************
         * Adds a dispatcher
         *
         * @param dispatcher dispatcher to add
         **********************************************************************************************/
        auto add_dispatcher(Ref<Provider> dispatcher) -> void
        {
            m_dispatchers.push_back(dispatcher);
        }

        /***********************************************************************************************
         * Adds a subscriber
         *
         * @param obj subscriber to be added
         **********************************************************************************************/
        auto add_subscriber(Ref<Subscriber> const &subscriber) -> void
        {
            m_subscribers.push_back(subscriber);
        }

        /***********************************************************************************************
         * Adds a callback to the mapped event
         *
         * @param event key for the callback
         * @param callback callback to be mapped in the list
         **********************************************************************************************/
        auto add_callback(Event const &event, Callback callback) -> void
        {
            m_actions[event].push_back(callback);
        }

        // ========================================================================================== //
        // Specific provider implementation ========================================================= //
        // ========================================================================================== //

        /***********************************************************************************************
         * Constraints the derived type and provides
         * the function declaration itself for any reference
         * to `m_dispatchers`
         **********************************************************************************************/
        auto dispatch(Event const & /*unused*/) -> void
        {
        }

        /***********************************************************************************************
         * Constraints the derived type and provides
         * the function declaration itself for any reference
         * to `m_dispatchers`
         **********************************************************************************************/
        auto emit(Event const & /*unused*/) -> void
        {
        }

        /***********************************************************************************************
         * Constraints the derived type and provides
         * the function declaration itself for any reference
         * to `m_dispatchers`
         **********************************************************************************************/
        auto listen(Event const & /*unused*/) -> void
        {
        }
    };
}  // namespace event
