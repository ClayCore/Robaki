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
    template <typename Type, typename Value = typename std::add_pointer<Type>::type>
    class Iterator
    {
    private:
        /** Lambda type alias. Used to mutate the iterator */
        using Transformer = std::function<Value(bool)>;

        /** Wrapper for the internal iterator */
        struct Wrapper
        {
            Iterator<Type> m_iter;
            typename Iterator<Type>::iterator m_first;
            typename Iterator<Type>::iterator m_last;

            explicit Wrapper(Iterator<Type> iter)
              : m_iter(iter), m_first(m_iter.begin()), m_last(m_iter.end()){};
        };

        /** Transformer function to mutate the underlying index */
        Transformer m_transformer;

    public:
        // ============================================================================================== //
        // Constructors ================================================================================= //
        // ============================================================================================== //

        Iterator() : m_transformer([](bool) { return (nullptr); })
        {
        }
        explicit Iterator(Transformer transformer) : m_transformer(transformer)
        {
        }

        struct iterator
        {
            // ========================================================================================== //
            // Utility type aliases ===================================================================== //
            // ========================================================================================== //

            using difference_type   = std::ptrdiff_t;
            using value_type        = Type;
            using pointer           = const Type *;
            using reference         = const Type &;
            using iterator_category = std::forward_iterator_tag;

            Transformer m_transformer;
            Value m_value;

            // ========================================================================================== //
            // Constructors ============================================================================= //
            // ========================================================================================== //

            iterator() : m_transformer([](bool) { return (nullptr); }), m_value(nullptr)
            {
            }
            explicit iterator(Value value) : m_transformer([](bool) { return (nullptr); }), m_value(value)
            {
            }
            explicit iterator(Transformer provider) : m_transformer(provider), m_value(m_transformer(false))
            {
            }

            // ========================================================================================== //
            // Operator overloads ======================================================================= //
            // ========================================================================================== //

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

            // ========================================================================================== //
            // Utility ref and deref methods ============================================================ //
            // ========================================================================================== //

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
            return (iterator(m_transformer));
        }
        auto end()
        {
            return (iterator(nullptr));
        }

        // ========================================================================================== //
        // Transformation and mutation methods ====================================================== //
        // ========================================================================================== //

        template <typename U>
        inline auto map(std::function<U(Type *)> func)
        {
            // clang-format off
            return (
                Iterator<U>([=, wrapper = Wrapper(*this), value = std::optional<U>()](bool advance) mutable {
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

        inline auto ptr()
        {
            // clang-format off
            return (
                this->map<Type *>([](Type *value) -> Type * { return (value); })
            );
            // clang-format on
        }

        inline auto ref()
        {
            // clang-format off
            return (
                this->map<std::reference_wrapper<Type>>([](Type *value) -> std::reference_wrapper<Type> {
                    return (std::ref(*value));
                })
            );
            //clang-format on
        }

        template <typename U = typename std::remove_pointer<Type>::type>
        requires std::is_pointer<Type>::value
        inline auto deref()
        {
            // clang-format off
            return (this->map<U>([](U **value) -> U {
                return (**value);
            }));
            // clang-format on
        }

        template <typename C, typename E = typename C::value_type>
        static inline auto from_container(C &container)
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

    template <typename Vector, typename Value = typename Vector::value_type>
    requires traits::is_vector_v<Vector>
    inline auto iter(Vector &vector)
    {
        return (Iterator<Value>::from_container(vector));
    }

    template <typename Map, typename Key = typename Map::key_type, typename Value = typename Map::value_type>
    inline auto iter_keys(Map &map)
    {
        return (Iterator<Value>::from_container(map).template map<Key *>(
            [](Value *map_object) -> Key * { return (&map_object->first); }));
    }

    template <typename Map,
              typename Mapped = typename Map::mapped_type,
              typename Value  = typename Map::value_type>
    inline auto iter_values(Map &map)
    {
        return (Iterator<Value>::from_container(map).template map<Mapped *>(
            [](Value *map_object) -> Mapped * { return (&map_object->second); }));
    }

}  // namespace util
