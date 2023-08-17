/** @file event.hpp */

#pragma once

// module includes
#include "util/util.hpp"

// fmt includes
#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif

#include "fmt/format.h"

namespace event
{
    /***************************************************************************************************
     * List of predefined events to for use with
     * the event handling system
     **************************************************************************************************/
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        LoadResource,
        FreeResource,
        EngineTick,
        EngineUpdate,
        EngineRender,
        KeyPressed,
        KeyReleased,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,
        Custom
    };

    /***************************************************************************************************
     * List of predefined event categories
     **************************************************************************************************/
    enum class EventCategory
    {
        None = 0,
        Window,
        Resource,
        Engine,
        Input,
        Custom
    };

    /***************************************************************************************************
     * Describes a singular event that can be fired, forwarded or subscribed to
     *
     * Can be derived and overriden for custom events.
     **************************************************************************************************/
    class Event
    {
    private:
        /** Defines the type of this event */
        EventType m_type{ EventType::None };

        /** Defines the category this event belongs to */
        EventCategory m_category{ EventCategory::None };

        /** Used for formatting, debugging and displaying */
        std::string m_name{ this->get_name_from_type(EventType::None) };

    public:
        // ========================================================================================== //
        // Constructors and destructors ============================================================= //
        // ========================================================================================== //

        Event()                         = default;
        Event(const Event &)            = default;
        Event(Event &&)                 = delete;
        Event &operator=(const Event &) = default;
        Event &operator=(Event &&)      = delete;
        explicit Event(EventType const &);
        virtual ~Event() = default;

        // ========================================================================================== //
        // Accessor methods ========================================================================= //
        // ========================================================================================== //

        /***********************************************************************************************
         * Provides the internal name of the event
         * Used by hashing functions
         *
         * @return event name
         **********************************************************************************************/
        [[nodiscard]] constexpr auto get_name() const -> char const *
        {
            if (std::empty(m_name)) {
                return (this->get_name_from_type(m_type));
            } else {
                return (m_name.c_str());
            }
        }

        /***********************************************************************************************
         * Returns the event's type
         *
         * @return event type
         **********************************************************************************************/
        [[nodiscard]] constexpr auto get_type() const -> EventType
        {
            return (m_type);
        }

        /**
         * Returns the category where the event belongs to
         *
         * @return event category
         */
        [[nodiscard]] constexpr auto get_category() const -> EventCategory
        {
            return (m_category);
        }

        // ========================================================================================== //
        // Transformation methods =================================================================== //
        // ========================================================================================== //

        /***********************************************************************************************
         * Transforms the event type into a string
         *
         * @return event type as string
         **********************************************************************************************/
        [[nodiscard]] virtual auto get_name_from_type(EventType const &) const -> char const *;

        /***********************************************************************************************
         * Transforms the event type into a category
         *
         * @return event type as category
         **********************************************************************************************/
        [[nodiscard]] virtual auto get_category_from_type(EventType const &) const -> EventCategory;

        /***********************************************************************************************
         * Transforms the category into a string
         *
         * @return event category as string
         **********************************************************************************************/
        [[nodiscard]] virtual auto category_to_string(EventCategory const &) const -> char const *;

        // ========================================================================================== //
        // Mutator methods ========================================================================== //
        // ========================================================================================== //

        /***********************************************************************************************
         * Sets the type of the event
         *
         * @param type type to set
         **********************************************************************************************/
        virtual auto set_type(EventType const &type) -> void;

        /***********************************************************************************************
         * Sets the name of the event
         *
         * @param name name to set
         **********************************************************************************************/
        virtual auto set_name(std::string const &name) -> void;

        /***********************************************************************************************
         * Sets the category of the event
         *
         * @param name category to set
         **********************************************************************************************/
        virtual auto set_category(EventCategory const &category) -> void;

        // ========================================================================================== //
        // Operator overloads ======================================================================= //
        // ========================================================================================== //

        constexpr auto operator==(Event const &rhs) const -> bool
        {
            return (this->get_name() == rhs.get_name());
        }

        constexpr auto operator!=(Event const &rhs) const -> bool
        {
            return (!(*this == rhs));
        }

        constexpr auto operator<=>(Event const &rhs) const -> std::strong_ordering
        {
            return (this->get_type() <=> rhs.get_type());
        }

        // ========================================================================================== //
        // Debugging methods ======================================================================== //
        // ========================================================================================== //

        /***********************************************************************************************
         * Converts the event structure into a string
         *
         * @return event as string
         **********************************************************************************************/
        [[nodiscard]] auto to_string() const -> std::string;

        /***********************************************************************************************
         * Allows writing the event string into a stream
         *
         * @param os stream to write to
         * @param event event to write
         * @return reference to used stream
         **********************************************************************************************/
        friend inline auto operator<<(std::ostream &ostream, Event const &event) -> std::ostream &
        {
            return (ostream << event.to_string());
        }
    };

    /***************************************************************************************************
     * Used by `std::unordered_map` when using `Event` as a key of the map
     **************************************************************************************************/
    struct EventHasher
    {
        auto operator()(Event const &) const -> usize;
    };

    /***************************************************************************************************
     * Used by `std::unordered_map` when using `Event` as a key of the map
     **************************************************************************************************/
    struct EventEqualizer
    {
        auto operator()(Event const &, Event const &) const -> bool;
    };
}  // namespace event

template <>
struct fmt::formatter<event::Event> : formatter<std::string>
{
    auto format(event::Event const &event, format_context &ctx) const -> format_context::iterator
    {
        return formatter<std::string>::format(event.to_string(), ctx);
    }
};
