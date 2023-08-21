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
            Iterator<Type> m_iter;
            Iterator<Type>::iterator m_first;
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

            using difference_type   = isize;
            using value_type        = Type;
            using pointer           = Type const *;
            using reference         = Type const &;
            using iterator_category = std::forward_iterator_tag;

            Transformer m_transformer;
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

            auto operator++(i32) -> iterator
            {
                auto iter = *this;
                ++*this;

                return (iter);
            }
            auto operator++() -> iterator &
            {
                m_value = m_transformer(true);

                return (*this);
            }

            auto operator==(iterator other) const -> bool
            {
                return (m_value == other.m_value);
            }
            auto operator!=(iterator other) const -> bool
            {
                return (!(*this == other));
            }

            // ====================================================================================== //
            // Utility ref and deref methods ======================================================== //
            // ====================================================================================== //

            auto operator*() -> Type &
            {
                return (*m_value);
            }
            auto raw() -> Type *&
            {
                return (m_value);
            }
        };

        // ========================================================================================== //
        // Accessor methods ========================================================================= //
        // ========================================================================================== //

        auto begin()
        {
            return (iterator { m_transformer });
        }
        auto end()
        {
            return (iterator { nullptr });
        }

        // ========================================================================================== //
        // Transformation and mutation methods ====================================================== //
        // ========================================================================================== //

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

        auto inline ptr()
        {
            // clang-format off
            return (
                this->map<Type *>([](Type *value) -> Type * { return (value); })
            );
            // clang-format on
        }

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

        template <class U = std::remove_pointer_t<Type>>
        requires std::is_pointer<Type>::value
        auto inline deref()
        {
            // clang-format off
            return (this->map<U>([](U **value) -> U {
                return (**value);
            }));
            // clang-format on
        }

        template <class C, class E = C::value_type>
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

    template <class Vector, class Value = Vector::value_type>
    requires traits::is_vector_v<Vector>
    auto inline iter(Vector &vector)
    {
        return (Iterator<Value>::from_container(vector));
    }

    template <class Map, class Key = Map::key_type, class Value = Map::value_type>
    auto inline iter_keys(Map &map)
    {
        return (Iterator<Value>::from_container(map).template map<Key *>(
            [](Value *map_object) -> Key * { return (&map_object->first); }));
    }

    template <typename Map, class Mapped = Map::mapped_type, class Value = Map::value_type>
    auto inline iter_values(Map &map)
    {
        return (Iterator<Value>::from_container(map).template map<Mapped *>(
            [](Value *map_object) -> Mapped * { return (&map_object->second); }));
    }

}  // namespace util
