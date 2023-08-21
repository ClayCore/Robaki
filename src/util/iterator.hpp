/** @file iterator.hpp */

#pragma once

// module includes
#include "traits.hpp"
#include "types.hpp"


// c++ includes
#include <functional>
#include <optional>
#include <unordered_map>
#include <utility>
#include <variant>


namespace util
{
    template <class Type, class Value = std::add_pointer_t<Type>>
    class Iterator
    {
    private:
        /** Lambda type alias. Used to mutate the iterator */
        using Transformer = std::function<Value(bool)>;

        /** Wrapper for the internal iterator */
        struct Wrapper
        {
            /** Internal iterator object */
            Iterator<Type> m_iter;

            /** Reference iterator to the beginning of the iterator */
            Iterator<Type>::iterator m_first;
            /** Reference iterator to the end of the iterator */
            Iterator<Type>::iterator m_last;

            explicit Wrapper(Iterator<Type> iter)
              : m_iter(iter), m_first(m_iter.begin()), m_last(m_iter.end()) {};
        };

        /** Transformer function to mutate the underlying index */
        Transformer m_transformer;

    public:
        // ========================================================================================== //
        // Constructors ============================================================================= //
        // ========================================================================================== //

        Iterator(): m_transformer { [](bool) { return (nullptr); } }
        {
        }
        explicit Iterator(Transformer transformer): m_transformer { transformer }
        {
        }

        struct iterator
        {
            // ====================================================================================== //
            // Utility type aliases ================================================================= //
            // ====================================================================================== //

            /** Used with `std::distance` */
            using difference_type = isize;
            /** Internal value type */
            using value_type = Type;
            /** Internal value pointer type */
            using pointer = Type const *;
            /** Internal value reference type */
            using reference = Type const &;
            /** Iterator classification */
            using iterator_category = std::forward_iterator_tag;

            /** Transformation method */
            Transformer m_transformer;

            /** Internal value */
            Value m_value;

            // ====================================================================================== //
            // Constructors ========================================================================= //
            // ====================================================================================== //

            iterator(): m_transformer { [](bool) { return (nullptr); } }, m_value { nullptr }
            {
            }
            explicit iterator(Value value)
              : m_transformer { [](bool) { return (nullptr); } }, m_value { value }
            {
            }
            explicit iterator(Transformer provider)
              : m_transformer { provider }, m_value { m_transformer(false) }
            {
            }

            // ====================================================================================== //
            // Operator overloads =================================================================== //
            // ====================================================================================== //

            /** Postincrement operator */
            auto operator++(i32) -> iterator
            {
                auto iter = *this;
                ++*this;

                return (iter);
            }
            /** Preincrement operator */
            auto operator++() -> iterator &
            {
                m_value = m_transformer(true);

                return (*this);
            }

            /** Equality comparison */
            auto operator==(iterator other) const -> bool
            {
                return (m_value == other.m_value);
            }

            /** Inequality comparison */
            auto operator!=(iterator other) const -> bool
            {
                return (!(*this == other));
            }

            /** Provides utility access to internal value */
            auto operator*() -> Type &
            {
                return (*m_value);
            }

            /** Acquire raw access to internal value */
            auto raw() -> Type *&
            {
                return (m_value);
            }
        };

        // ========================================================================================== //
        // Accessor methods ========================================================================= //
        // ========================================================================================== //

        /** Iterator `std::begin` overload */
        auto begin()
        {
            return (iterator { m_transformer });
        }

        /** Iterator `std::end` overload */
        auto end()
        {
            return (iterator { nullptr });
        }

        // ========================================================================================== //
        // Transformation and mutation methods ====================================================== //
        // ========================================================================================== //

        /***********************************************************************************************
         * Mutates the internal value of the iterator using the provided map function
         *
         * @param func transformation function
         * @returns value when succesful, `nullptr` otherwise
         **********************************************************************************************/
        template <class U>
        auto inline map(std::function<U(Type *)> func)
        {
            // clang-format off
            return (
                Iterator<U>([=, wrapper {Wrapper(*this)}, value {std::optional<U>()}](bool advance) mutable {
                    if (advance) {
                        ++wrapper.m_first;
                    }

                    return (
                        (wrapper.m_first != wrapper.m_last) ?
                        (&*(value = func(wrapper.m_first.raw()))) :
                        (nullptr)
                    );
                })
            );
            // clang-format on
        }

        /***********************************************************************************************
         * Acquire pointer to value
         *
         * @return pointer to value
         **********************************************************************************************/
        auto inline ptr()
        {
            // clang-format off
            return (
                this->map<Type *>([](Type *value) -> Type * { return (value); })
            );
            // clang-format on
        }

        /***********************************************************************************************
         * Acquire reference to value
         *
         * @return reference to value
         **********************************************************************************************/
        auto inline ref()
        {
            // clang-format off
            return (
                this->map<std::reference_wrapper<Type>>([](Type *value) -> std::reference_wrapper<Type> {
                    return (std::ref(*value));
                })
            );
            //clang-format on
        }

        /***********************************************************************************************
         * Dereference inner value. Only valid when `T` is a pointer.
         *
         * @tparam U `T` with removed pointer qualifier
         * @returns dereferenced value when valid, `nullptr` otherwise
         **********************************************************************************************/
        template <class U = std::remove_pointer_t<Type>>
        requires std::is_pointer_v<Type>
        auto inline deref()
        {
            // clang-format off
            return (this->map<U>([](U **value) -> U {
                return (**value);
            }));
            // clang-format on
        }

        /***********************************************************************************************
         * Constructs `Iterator` from an STL container
         *
         * @tparam C container type
         * @param container container to construct iterator from
         * @returns iterator when valid, `nullptr` otherwise
         **********************************************************************************************/
        template <IsContainer C, class E = C::value_type>
        auto static inline from_container(C &container)
        {
            // clang-format off
            return (
                Iterator<E>([iter = container.begin(), end = container.end()](bool advance) mutable -> E * {
                    if (advance) {
                        ++iter;
                    }

                    return ((iter != end) ? (&(*iter)) : (nullptr));
                })
            );
            // clang-format on
        }
    };

    /***************************************************************************************************
     * Constructs an Iterator from an `std::vector`
     *
     * @tparam Vector vector type
     * @tparam Value vector value type
     * @param vector vector to construct iterator from
     * @return iterator when valid, `nullptr` otherwise
     **************************************************************************************************/
    template <IsVector Vector, class Value = Vector::value_type>
    auto inline iter(Vector &vector)
    {
        return (Iterator<Value>::from_container(vector));
    }

    /***************************************************************************************************
     * Constructs an Iterator from keys in a map
     *
     * @tparam Map map container type
     * @tparam Key map key type
     * @tparam Value map value type
     * @param map map
     * @returns iterator from keys when valid, `nullptr` otherwise
     **************************************************************************************************/
    template <class Map, class Key = Map::key_type, class Value = Map::value_type>
    auto inline iter_keys(Map &map)
    {
        return (Iterator<Value>::from_container(map).template map<Key *>(
            [](Value *map_object) -> Key * { return (&map_object->first); }));
    }

    /***************************************************************************************************
     * Constructs an Iterator from value in a map
     *
     * @tparam Map map container type
     * @tparam Mapped map mapped type
     * @tparam Value map value type
     * @param map map
     * @returns iterator from value when valid, `nullptr` otherwise
     **************************************************************************************************/
    template <typename Map, class Mapped = Map::mapped_type, class Value = Map::value_type>
    auto inline iter_values(Map &map)
    {
        return (Iterator<Value>::from_container(map).template map<Mapped *>(
            [](Value *map_object) -> Mapped * { return (&map_object->second); }));
    }

}  // namespace util
