/** @file result.hpp */

#pragma once

// module includes
#include "traits.hpp"
#include "types.hpp"

// c++ includes
#include <functional>

namespace util::result
{
    namespace types
    {
        template <typename T>
        struct Ok
        {
            T m_value;

            explicit Ok(T const &value) : m_value(value)
            {
            }
            explicit Ok(T &&value) : m_value(std::move(value))
            {
            }
        };

        template <>
        struct Ok<void>
        {
        };

        template <typename E>
        struct Err
        {
            E m_error;

            explicit Err(E const &error) : m_error(error)
            {
            }
            explicit Err(E &&error) : m_error(std::move(error))
            {
            }
        };
    }  // namespace types

    template <typename T, typename Decay = typename std::decay_t<T>>
    constexpr auto Ok(T &&value) -> types::Ok<Decay>
    {
        return { types::Ok<Decay>{ std::forward<T>(value) } };
    }

    constexpr auto Ok() -> types::Ok<void>
    {
        return { types::Ok<void>{} };
    }

    template <typename E, typename Decay = typename std::decay_t<E>>
    constexpr auto Err(E &&error) -> types::Err<Decay>
    {
        return { types::Err<Decay>{ std::forward<E>(error) } };
    }

    template <typename T, typename E>
    struct Result;

    namespace details::impl
    {
        template <typename Func>
        struct result_of;

        template <typename Ret, typename Cls, typename... Args>
        struct result_of<Ret (Cls::*)(Args...)> : public result_of<Ret(Args...)>
        {
        };

        template <typename Ret, typename... Args>
        struct result_of<Ret(Args...)>
        {
            using type = Ret;
        };
    }  // namespace details::impl

    namespace details
    {
        struct ok_tag
        {
        };
        struct err_tag
        {
        };

        template <typename T, typename E>
        struct Storage
        {
            static constexpr const usize Size  = ((sizeof(T) > sizeof(E)) ? (sizeof(T)) : (sizeof(E)));
            static constexpr const usize Align = ((sizeof(T) > sizeof(E)) ? (alignof(T)) : (alignof(E)));

            using type = typename std::aligned_storage_t<Size, Align>;

            type m_storage     = {};
            bool m_initialized = { false };

            Storage() = default;

            template <typename U>
            auto get() const -> U const &
            {
                return (static_cast<U const *>(&m_storage));
            }

            template <typename U>
            auto get() -> U &
            {
                return (static_cast<U *>(&m_storage));
            }

            auto construct(types::Ok<T> ok_val) -> void
            {
                m_storage     = &(new T{ ok_val.m_value });
                m_initialized = true;
            }

            auto construct(types::Err<E> err_val) -> void
            {
                m_storage     = &(new T{ err_val.m_value });
                m_initialized = true;
            }

            template <typename U>
            auto raw_construct(U &&val) -> void
            {
                using Decay = typename std::decay_t<U>;

                m_storage     = &(new Decay{ std::forward<U>(val) });
                m_initialized = true;
            }

            auto destroy(ok_tag /* unused */) -> void
            {
                if (m_initialized) {
                    get<T>().~T();
                    m_initialized = false;
                }
            }

            auto destroy(err_tag /* unused */) -> void
            {
                if (m_initialized) {
                    get<E>().~E();
                    m_initialized = false;
                }
            }
        };

        template <typename E>
        struct Storage<void, E>
        {
            using type = typename std::aligned_storage_t<sizeof(E), alignof(E)>;

            type m_storage;
            bool m_initialized;

            template <typename U>
            auto get() const -> U const &
            {
                return (static_cast<U const *>(&m_storage));
            }

            template <typename U>
            auto get() -> U &
            {
                return (static_cast<U *>(&m_storage));
            }

            auto construct(types::Ok<void> /* unused */) -> void
            {
                m_initialized = true;
            }

            auto construct(types::Err<E> err_val) -> void
            {
                m_storage     = &(new E{ err_val.m_value });
                m_initialized = true;
            }

            template <typename U>
            auto raw_construct(U &&val) -> void
            {
                using Decay = typename std::decay_t<U>;

                m_storage     = &(new Decay{ std::forward<U>(val) });
                m_initialized = true;
            }

            auto destroy(ok_tag /* unused */) -> void
            {
                m_initialized = false;
            }

            auto destroy(err_tag /* unused */) -> void
            {
                if (m_initialized) {
                    get<E>().~E();
                    m_initialized = false;
                }
            }
        };

        template <typename T, typename E>
        struct Constructor
        {
            static auto move(Storage<T, E> &&src, Storage<T, E> &dst, ok_tag /* unused */) -> void
            {
                dst.raw_construct(std::move(src.template get<T>()));
                src.destroy(ok_tag{});
            }

            static auto move(Storage<T, E> &&src, Storage<T, E> &dst, err_tag /* unused */) -> void
            {
                dst.raw_construct(std::move(src.template get<E>()));
                src.destroy(err_tag{});
            }

            static auto copy(Storage<T, E> const &src, Storage<T, E> &dst, ok_tag /* unused */) -> void
            {
                dst.raw_construct(src.template get<T>());
            }

            static auto copy(Storage<T, E> const &src, Storage<T, E> &dst, err_tag /* unused */) -> void
            {
                dst.raw_construct(src.template get<E>());
            }
        };

        template <typename E>
        struct Constructor<void, E>
        {
            static auto move(Storage<void, E> && /*unused*/,
                             Storage<void, E> & /*unused*/,
                             ok_tag /* unused */) -> void
            {
            }

            static auto move(Storage<void, E> &&src, Storage<void, E> &dst, err_tag /* unused */) -> void
            {
                dst.raw_construct(std::move(src.template get<E>()));
                src.destroy(err_tag{});
            }

            static auto copy(Storage<void, E> const & /* unused */,
                             Storage<void, E> & /* unused */,
                             ok_tag /* unused */) -> void
            {
            }

            static auto copy(Storage<void, E> const &src, Storage<void, E> &dst, err_tag /* unused */) -> void
            {
                dst.raw_construct(src.template get<E>());
            }
        };

        template <typename...>
        struct void_t
        {
            using type = void_t;
        };

        template <typename Func>
        struct result_of : public impl::result_of<decltype(&Func::operator())>
        {
        };

        template <typename Ret, typename Cls, typename... Args>
        struct result_of<Ret (Cls::*)(Args...) const>
        {
            using type = Ret;
        };

        template <typename Ret, typename... Args>
        struct result_of<Ret (*)(Args...)>
        {
            using type = Ret;
        };

        template <typename Func>
        using result_of_t = result_of<Func>;

        template <typename R>
        struct ResultOkType
        {
            using type = typename std::decay_t<R>;
        };

        template <typename T, typename E>
        struct ResultOkType<Result<T, E>>
        {
            using type = T;
        };

        template <typename R>
        struct ResultErrType
        {
            using type = R;
        };

        template <typename T, typename E>
        struct ResultErrType<Result<T, E>>
        {
            using type = std::remove_reference<E>;
        };

        template <typename R>
        struct is_result : public std::false_type
        {
        };

        template <typename T, typename E>
        struct is_result<Result<T, E>> : public std::true_type
        {
        };

        template <typename T>
        constexpr const bool is_result_v = is_result<T>::value;

        namespace Ok::impl
        {
            template <typename T>
            struct Map;

            template <typename Ret, typename Cls, typename Arg>
            struct Map<Ret (Cls::*)(Arg) const> : public Map<Ret(Arg)>
            {
            };

            template <typename Ret, typename Cls, typename Arg>
            struct Map<Ret (Cls::*)(Arg)> : public Map<Ret(Arg)>
            {
            };

            template <typename Ret, typename Arg>
            struct Map<Ret(Arg)>
            {
                static_assert(!is_result_v<Ret>,
                              "Cannot map callback returning Result. Use `and_then` instead");

                template <typename T, typename E, typename Func>
                static auto map(Result<T, E> const &result, Func func) -> Result<Ret, E>
                {
                    static_assert(std::is_same_v<T, Arg> || std::is_convertible_v<T, Arg>,
                                  "Incompatible types detected");

                    if (result.is_ok()) {
                        auto res = func(result.storage().template get<T>());

                        return { types::Ok<Ret>{ std::move(res) } };
                    }

                    return { types::Err<E>{ result.storage().template get<E>() } };
                }
            };

            template <typename Arg>
            struct Map<void(Arg)>
            {
                template <typename T, typename E, typename Func>
                static auto map(Result<T, E> const &result, Func func) -> Result<void, E>
                {
                    if (result.is_ok()) {
                        func(result.storage().template get<T>());

                        return { types::Ok<void>{} };
                    }

                    return { types::Err<E>{ result.storage().template get<E>() } };
                }
            };

            template <typename Ret>
            struct Map<Ret(void)>
            {
                template <typename T, typename E, typename Func>
                static auto map(Result<T, E> const &result, Func func) -> Result<Ret, E>
                {
                    static_assert(std::is_same_v<T, void>, "Cannot map a void callback on a non-void Result");

                    if (result.is_ok()) {
                        func(result.storage().template get<T>());

                        return { types::Ok<void>{} };
                    }

                    return { types::Err<E>{ result.storage().template get<E>() } };
                }
            };

            template <typename U, typename E, typename Arg>
            struct Map<Result<U, E>(Arg)>
            {
                template <typename T, typename Func>
                static auto map(Result<T, E> const &result, Func func) -> Result<U, E>
                {
                    static_assert(std::is_same_v<T, Arg> || std::is_convertible_v<T, Arg>,
                                  "Incompatible types detected");

                    if (result.is_ok()) {
                        auto res = func(result.storage().template get<T>());

                        return { res };
                    }

                    return { types::Err<E>{ result.storage().template get<E>() } };
                }
            };

            template <typename U, typename E>
            struct Map<Result<U, E>(void)>
            {
                template <typename T, typename Func>
                static auto map(Result<T, E> const &result, Func func) -> Result<U, E>
                {
                    static_assert(std::is_same_v<T, void>,
                                  "Cannot call a void callback on a non-void Result");

                    if (result.is_ok()) {
                        auto res = func();

                        return { res };
                    }

                    return { types::Err<E>{ result.storage().template get<E>() } };
                }
            };

            template <>
            struct Map<void(void)>
            {
                template <typename T, typename E, typename Func>
                static auto map(Result<T, E> const &result, Func func) -> Result<void, E>
                {
                    static_assert(std::is_same_v<T, void>,
                                  "Cannot map a void callback on a non-void `Result`");

                    if (result.is_ok()) {
                        func();
                        return { types::Ok<void>() };
                    }

                    return { types::Err<E>{ result.storage().template get<E>() } };
                }
            };
        }  // namespace Ok::impl

        namespace Ok
        {
            template <typename Func>
            struct Map : public impl::Map<decltype(&Func::operator())>
            {
            };

            template <typename Ret, typename... Args>
            struct Map<Ret (*)(Args...)> : public impl::Map<Ret(Args...)>
            {
            };

            template <typename Ret, typename Cls, typename... Args>
            struct Map<Ret (Cls::*)(Args...)> : public impl::Map<Ret(Args...)>
            {
            };

            template <typename Ret, typename Cls, typename... Args>
            struct Map<Ret (Cls::*)(Args...) const> : public impl::Map<Ret(Args...)>
            {
            };

            template <typename Ret, typename... Args>
            struct Map<std::function<Ret(Args...)>> : public impl::Map<Ret(Args...)>
            {
            };
        }  // namespace Ok

        namespace Err::impl
        {
            template <typename T>
            struct Map;

            template <typename Ret, typename Cls, typename Arg>
            struct Map<Ret (Cls::*)(Arg) const>
            {
                static_assert(!is_result_v<Ret>,
                              "Cannot map a callback returning `Result`. Use `or_else` instead");

                template <typename T, typename E, typename Func>
                static auto map(Result<T, E> const &result, Func func) -> Result<T, Ret>
                {
                    if (result.is_err()) {
                        auto res = func(result.storage().template get<E>());

                        return { types::Err<Ret>{ res } };
                    }

                    return { types::Ok<T>{ result.storage().template get<T>() } };
                }

                template <typename E, typename Func>
                static auto map(Result<void, E> const &result, Func func) -> Result<void, Ret>
                {
                    if (result.is_err()) {
                        auto res = func(result.storage().template get<E>());

                        return { types::Err<Ret>{ res } };
                    }

                    return { types::Ok<void>{} };
                }
            };
        }  // namespace Err::impl

        namespace Err
        {
            template <typename Func>
            struct Map : public impl::Map<decltype(&Func::operator())>
            {
            };
        }  // namespace Err

        namespace And::impl
        {
            template <typename Func>
            struct Then;

            template <typename Ret, typename... Args>
            struct Then<Ret (*)(Args...)> : public Then<Ret(Args...)>
            {
            };

            template <typename Ret, typename Cls, typename... Args>
            struct Then<Ret (Cls::*)(Args...)> : public Then<Ret(Args...)>
            {
            };

            template <typename Ret, typename Cls, typename... Args>
            struct Then<Ret (Cls::*)(Args...) const> : public Then<Ret(Args...)>
            {
            };

            template <typename Ret, typename Arg>
            struct Then<Ret(Arg)>
            {
                static_assert(std::is_same_v<Ret, void>,
                              "`then` should not return anything, use `map` instead");

                template <typename T, typename E, typename Func>
                static auto then(Result<T, E> const &result, Func func) -> Result<T, E>
                {
                    if (result.is_ok()) {
                        func(result.storage().template get<T>());
                    }

                    return { result };
                }
            };

            template <typename Ret>
            struct Then<Ret(void)>
            {
                static_assert(std::is_same_v<Ret, void>,
                              "`then` should not return anything, use `map` instead");

                template <typename T, typename E, typename Func>
                static auto then(Result<T, E> const &result, Func func) -> Result<T, E>
                {
                    static_assert(std::is_same_v<T, void>,
                                  "Cannot call a void callback on a non-void `Result`");

                    if (result.is_ok()) {
                        func();
                    }

                    return { result };
                }
            };
        }  // namespace And::impl

        namespace And
        {
            template <typename Func>
            struct Then : public impl::Then<decltype(&Func::operator())>
            {
            };

            template <typename Ret, typename... Args>
            struct Then<Ret (*)(Args...)> : public impl::Then<Ret(Args...)>
            {
            };

            template <typename Ret, typename Cls, typename... Args>
            struct Then<Ret (Cls::*)(Args...)> : public impl::Then<Ret(Args...)>
            {
            };

            template <typename Ret, typename Cls, typename... Args>
            struct Then<Ret (Cls::*)(Args...) const> : public impl::Then<Ret(Args...)>
            {
            };
        }  // namespace And

        namespace Or::impl
        {
            template <typename Func>
            struct Else;

            template <typename Ret, typename... Args>
            struct Else<Ret (*)(Args...)> : public Else<Ret(Args...)>
            {
            };

            template <typename Ret, typename Cls, typename... Args>
            struct Else<Ret (Cls::*)(Args...)> : public Else<Ret(Args...)>
            {
            };

            template <typename Ret, typename Cls, typename... Args>
            struct Else<Ret (Cls::*)(Args...) const> : public Else<Ret(Args...)>
            {
            };

            template <typename T, typename F, typename Arg>
            struct Else<Result<T, F>(Arg)>
            {
                template <typename E, typename Func>
                static auto or_else(Result<T, E> const &result, Func func) -> Result<T, F>
                {
                    static_assert(std::is_same_v<E, Arg> || std::is_convertible_v<E, Arg>,
                                  "Incompatible types detected");

                    if (result.is_err()) {
                        auto res = func(result.storage().template get<E>());

                        return { res };
                    }

                    return { types::Ok<T>{ result.storage().template get<T>() } };
                }

                template <typename E, typename Func>
                static auto or_else(Result<void, E> const &result, Func func) -> Result<void, F>
                {
                    if (result.is_err()) {
                        auto res = func(result.storage().template get<E>());

                        return { res };
                    }

                    return { types::Ok<void>{} };
                }
            };

            template <typename T, typename F>
            struct Else<Result<T, F>(void)>
            {
                template <typename E, typename Func>
                static auto or_else(Result<T, E> const &result, Func func) -> Result<T, F>
                {
                    static_assert(std::is_same_v<T, void>,
                                  "Cannot call a void callback on a non-void `Result`");

                    if (result.is_err()) {
                        auto res = func();

                        return { res };
                    }

                    return { types::Ok<T>{ result.storage().template get<T>() } };
                }

                template <typename E, typename Func>
                static auto or_else(Result<void, E> const &result, Func func) -> Result<void, F>
                {
                    if (result.is_err()) {
                        auto res = func();

                        return { res };
                    }

                    return { types::Ok<void>{} };
                }
            };

        }  // namespace Or::impl

        namespace Or
        {
            template <typename Func>
            struct Else : public impl::Else<decltype(&Func::operator())>
            {
            };

            template <typename Ret, typename... Args>
            struct Else<Ret (*)(Args...)> : public impl::Else<Ret(Args...)>
            {
            };

            template <typename Ret, typename Cls, typename... Args>
            struct Else<Ret (Cls::*)(Args...)> : public impl::Else<Ret(Args...)>
            {
            };

            template <typename Ret, typename Cls, typename... Args>
            struct Else<Ret (Cls::*)(Args...) const> : public impl::Else<Ret(Args...)>
            {
            };
        }  // namespace Or

        namespace Other::impl
        {
            template <typename Func>
            struct Wise;

            template <typename Ret, typename... Args>
            struct Wise<Ret (*)(Args...)> : public Wise<Ret(Args...)>
            {
            };

            template <typename Ret, typename Cls, typename... Args>
            struct Wise<Ret (Cls::*)(Args...)> : public Wise<Ret(Args...)>
            {
            };

            template <typename Ret, typename Cls, typename... Args>
            struct Wise<Ret (Cls::*)(Args...) const> : public Wise<Ret(Args...)>
            {
            };

            template <typename Ret, typename Arg>
            struct Wise<Ret(Arg)>
            {
                template <typename T, typename E, typename Func>
                static auto otherwise(Result<T, E> const &result, Func func) -> Result<T, E>
                {
                    static_assert(std::is_same_v<E, Arg> || std::is_convertible_v<E, Arg>,
                                  "Incompatible types detected");

                    static_assert(std::is_same_v<Ret, void>,
                                  "Callback should not return anything, use `map_error` for that");

                    if (result.is_err()) {
                        func(result.storage().template get<E>());
                    }

                    return { result };
                }
            };
        }  // namespace Other::impl

        namespace Other
        {
            template <typename Func>
            struct Wise : public impl::Wise<decltype(&Func::operator())>
            {
            };

            template <typename Ret, typename... Args>
            struct Wise<Ret (*)(Args...)> : public impl::Wise<Ret(Args...)>
            {
            };

            template <typename Ret, typename Cls, typename... Args>
            struct Wise<Ret (Cls::*)(Args...)> : public impl::Wise<Ret(Args...)>
            {
            };

            template <typename Ret, typename Cls, typename... Args>
            struct Wise<Ret (Cls::*)(Args...) const> : public impl::Wise<Ret(Args...)>
            {
            };
        }  // namespace Other

        template <typename T,
                  typename E,
                  typename Func,
                  typename Ret =
                      Result<typename details::ResultOkType<typename details::result_of_t<Func>>::type, E>>
        auto map(Result<T, E> const &result, Func func) -> Ret
        {
            return { Ok::Map<Func>::map(result, func) };
        }

        template <typename T,
                  typename E,
                  typename Func,
                  typename Ret =
                      Result<T, typename details::ResultErrType<typename details::result_of_t<Func>>::type>>
        auto map_error(Result<T, E> const &result, Func func) -> Ret
        {
            return { Err::Map<Func>::map(result, func) };
        }

        template <typename T, typename E, typename Func>
        auto then(Result<T, E> const &result, Func func) -> Result<T, E>
        {
            return { And::Then<Func>::then(result, func) };
        }

        template <typename T, typename E, typename Func>
        auto otherwise(Result<T, E> const &result, Func func) -> Result<T, E>
        {
            return { Other::Wise<Func>::otherwise(result, func) };
        }

        template <typename T,
                  typename E,
                  typename Func,
                  typename Ret =
                      Result<T, typename details::ResultErrType<typename details::result_of_t<Func>>::type>>
        auto or_else(Result<T, E> const &result, Func func) -> Ret
        {
            return { Or::Else<Func>::or_else(result, func) };
        }
    }  // namespace details

    // TODO: move to traits.hpp
    namespace concepts
    {
        template <typename T, typename = void>
        struct EqualityComparable : std::false_type
        {
        };

        template <typename T>
        struct EqualityComparable<
            T,
            std::enable_if_t<
                true,
                typename details::void_t<decltype(std::declval<T>() == std::declval<T>())>::type>>
          : std::true_type
        {
        };
    }  // namespace concepts

    template <typename T, typename E>
    struct Result
    {
    public:
        using storage_type = details::Storage<T, E>;

        template <typename U = T, typename V = E>
        using unwrap_result_type_str =
            std::enable_if_t<!std::is_same_v<U, void> && std::is_same_v<V, std::string>, U>;

        template <typename U = T, typename V = E>
        using unwrap_result_type =
            std::enable_if_t<!std::is_same_v<U, void> && !std::is_same_v<V, std::string>, U>;

        static_assert(!std::is_same_v<E, void>, "void `Error` type is not allowed");

        struct UnwrapOptions
        {
            char const *file   = { nullptr };
            char const *caller = { nullptr };
            usize line         = { 0 };

            UnwrapOptions() : file(__builtin_FILE()), caller(__builtin_FUNCTION()), line(__builtin_LINE())
            {
            }
        };

    private:
        bool m_ok;
        storage_type m_storage;

        constexpr auto expect_impl(std::true_type /* unused */) const -> T
        {
        }

        constexpr auto expect_impl(std::false_type /* unused */) const -> T
        {
            return { m_storage.template get<T>() };
        }

    public:
        explicit Result(types::Ok<T> ok_val) : m_ok(true)
        {
            m_storage.construct(std::move(ok_val));
        }

        explicit Result(types::Err<E> err_val) : m_ok(false)
        {
            m_storage.construct(std::move(err_val));
        }

        Result(Result &&other) noexcept
        {
            if (other.is_ok()) {
                details::Constructor<T, E>::move(std::move(other.m_storage), m_storage, details::ok_tag{});
                m_ok = true;
            } else {
                details::Constructor<T, E>::move(std::move(other.m_storage), m_storage, details::err_tag{});
                m_ok = false;
            }
        }

        Result(Result const &other) noexcept
        {
            if (other.is_ok()) {
                details::Constructor<T, E>::copy(other.m_storage, m_storage, details::ok_tag{});
                m_ok = true;
            } else {
                details::Constructor<T, E>::copy(other.m_storage, m_storage, details::err_tag{});
                m_ok = false;
            }
        }

        ~Result()
        {
            if (m_ok) {
                m_storage.destroy(details::ok_tag{});
            } else {
                m_storage.destroy(details::err_tag{});
            }
        }

        [[nodiscard]] auto storage() -> storage_type &
        {
            return { m_storage };
        }

        [[nodiscard]] auto storage() const -> storage_type const &
        {
            return { m_storage };
        }

        [[nodiscard]] auto is_ok() const -> bool
        {
            return { m_ok };
        }

        [[nodiscard]] auto is_err() const -> bool
        {
            return { !m_ok };
        }

        auto expect(char const * /* unused */) const -> T
        {
            if (!m_ok) {
                // terminate and print error
            }

            return { this->expect_impl(std::is_same_v<T, void>()) };
        }

        template <typename Func,
                  typename Ret =
                      Result<typename details::ResultOkType<typename details::result_of_t<Func>>::type, E>>
        Ret map(Func func) const
        {
            return details::map(*this, func);
        }

        template <typename Func,
                  typename Ret =
                      Result<T, typename details::ResultErrType<typename details::result_of_t<Func>>::type>>
        Ret map_error(Func func) const
        {
            return details::map_error(*this, func);
        }

        template <typename Func>
        Result<T, E> then(Func func) const
        {
            return details::then(*this, func);
        }

        template <typename Func>
        Result<T, E> otherwise(Func func) const
        {
            return details::otherwise(*this, func);
        }

        template <
            typename Func,
            typename Ret =
                Result<T, typename details::ResultErrType<typename details::result_of_t<Func>::type>::type>>
        Ret or_else(Func func) const
        {
            return details::or_else(*this, func);
        }

        template <typename U = T, typename V = E>
        [[nodiscard]] auto unwrap(UnwrapOptions /* unused */) const -> unwrap_result_type<U, T>
        {
            if (is_ok()) {
                return { this->storage().template get<U>() };
            }

            // todo: log and terminate
        }

        template <typename U = T, typename V = E>
        [[nodiscard]] auto unwrap(UnwrapOptions /* unused */) const -> unwrap_result_type_str<U, T>
        {
            if (is_ok()) {
                return { this->storage().template get<U>() };
            }

            // todo: log and terminate
        }

        [[nodiscard]] auto unwrap_err(UnwrapOptions /* unused */) const -> E
        {
            if (is_err()) {
                return { this->storage().template get<E>() };
            }

            // todo: log and terminate
        }

        friend auto operator==(Result<T, E> const &lhs, Result<T, E> const &rhs) -> bool
        {
            static_assert(concepts::EqualityComparable<T>::value,
                          "T must be `EqualityComparable` for `Result` to be comparable");

            static_assert(concepts::EqualityComparable<E>::value,
                          "E must be `EqualityComparable` for `Result` to be comparable");

            if (lhs.is_ok() && rhs.is_ok()) {
                return { (lhs.storage().template get<T>()) == (rhs.storage().template get<T>()) };
            }
            if (lhs.is_err() && rhs.is_err()) {
                return { (lhs.storage().template get<E>()) == (rhs.storage().template get<E>()) };
            }
        }

        friend auto operator==(Result<T, E> const &lhs, types::Ok<T> &rhs) -> bool
        {
            static_assert(concepts::EqualityComparable<T>::value,
                          "T must be `EqualityComparable` for `Result` to be comparable");

            if (!lhs.is_ok()) {
                return { false };
            }

            return { (lhs.storage().template get<T>()) == (rhs.m_value) };
        }

        friend auto operator==(Result<void, E> const &lhs, types::Ok<void> & /* unused */) -> bool
        {
            return { lhs.is_ok() };
        }

        friend auto operator==(Result<T, E> const &lhs, types::Err<E> err) -> bool
        {
            static_assert(concepts::EqualityComparable<E>::value,
                          "E must be `EqualityComparable` for `Result` to be comparable");

            if (!lhs.is_err()) {
                return { false };
            }

            return { (lhs.storage().template get<E>()) == (err.m_value) };
        }
    };
}  // namespace util::result
