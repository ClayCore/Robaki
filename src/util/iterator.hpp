/** @file iterator.hpp */

#pragma once

// module includes
#include "traits.hpp"
#include "types.hpp"


// c++ includes
#include <functional>
#include <optional>
#include <unordered_map>
#include <variant>
#include <xutility>


namespace util
{
    template <typename T, typename V = typename std::add_pointer<T>::type>
    class Iterator
    {
    private:
        using Provider = std::function<V(bool)>;

        struct Wrapper
        {
            Iterator<T> m_iter;
            typename Iterator<T>::iterator m_begin;
            typename Iterator<T>::iterator m_end;

            explicit Wrapper(Iterator<T> iter) : m_iter(iter), m_begin(m_iter.begin()), m_end(m_iter.end()){};
        };

        Provider m_provider;

    public:
        Iterator() : m_provider([](bool) { return (nullptr); })
        {
        }
        explicit Iterator(Provider provider) : m_provider(provider)
        {
        }

        struct iterator
        {
            using difference_type   = std::ptrdiff_t;
            using value_type        = T;
            using pointer           = const T *;
            using reference         = const T &;
            using iterator_category = std::forward_iterator_tag;

            Provider m_provider;
            V m_value;

            iterator() : m_provider([](bool) { return (nullptr); }), m_value(nullptr)
            {
            }
            explicit iterator(V value) : m_provider([](bool) { return (nullptr); }), m_value(value)
            {
            }
            explicit iterator(Provider provider) : m_provider(provider), m_value(m_provider(false))
            {
            }

            auto operator++(i32) -> iterator
            {
                auto iter = *this;
                ++*this;

                return (iter);
            }
            auto operator++() -> iterator &
            {
                m_value = m_provider(true);

                return (*this);
            }

            auto operator==(iterator const &other) const -> bool
            {
                return (m_value == other.m_value);
            }
            auto operator!=(iterator const &other) const -> bool
            {
                return (!(*this == other));
            }

            auto operator*() -> T &
            {
                return (*m_value);
            }
            auto raw() -> T *&
            {
                return m_value;
            }
        };

        auto begin()
        {
            return (iterator(m_provider));
        }
        auto end()
        {
            return (iterator(nullptr));
        }

        template <typename U>
        inline auto map(std::function<U(T *)> func)
        {
            return Iterator<U>([=, wrapper = Wrapper(*this), obj = std::optional<U>()](bool advance) mutable {
                if (advance) {
                    ++wrapper.m_iter;
                }

                return ((wrapper.m_iter != wrapper.m_end) ? (&*(obj = func(wrapper.m_iter.raw()))) : nullptr);
            });
        }

        inline auto ptr()
        {
            return (this->map<T *>([](T *obj) -> T * { return obj; }));
        }

        inline auto ref()
        {
            return (this->map<std::reference_wrapper<T>>(
                [](T *obj) -> std::reference_wrapper<T> { return (std::ref(*obj)); }));
        }

        template <typename U = typename std::remove_pointer<T>::type>
        requires std::is_pointer<T>::value
        inline auto deref()
        {
            return (this->map<U>([](U **obj) -> U { return **obj; }));
        }

        template <typename C, typename E = typename C::value_type>
        static inline auto from_container(C &container)
        {
            return (
                Iterator<E>([iter = container.begin(), end = container.end()](bool advance) mutable -> E * {
                    if (advance) {
                        ++iter;
                    }

                    return (iter != end ? &(*iter) : nullptr);
                }));
        }
    };

    template <typename V, typename T = typename V::value_type>
    requires traits::is_vector_v<V>
    inline auto iter(V &vector)
    {
        return (Iterator<T>::from_container(vector));
    }

    template <typename M, typename K = typename M::key_type, typename P = typename M::value_type>
    inline auto iter_keys(M &map)
    {
        return (Iterator<P>::from_container(map).template map<K *>(
            [](P *pair) -> K * { return (&pair->first); }));
    }

    template <typename M, typename V = typename M::mapped_type, typename P = typename M::value_type>
    inline auto iter_values(M &map)
    {
        return (Iterator<P>::from_container(map).template map<V *>(
            [](P *pair) -> V * { return (&pair->second); }));
    }

}  // namespace util
