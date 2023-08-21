/** @file buffer_ref.hpp */

// module includes
#include "buffer_ref.hpp"
#include "memory/buffer_ref.hpp"
#include <type_traits>

namespace memory
{
    template <class Ptr>
    inline BufferRef::BufferRef(Ptr *data, usize length)
      : m_data { reinterpret_cast<void *>(data) }, m_length { length }
    {
    }

    template <class Ptr>
    inline BufferRef::BufferRef(Ptr const *data, usize length)
      : m_data { reinterpret_cast<void *>(const_cast<Ptr *>(data)) }, m_length(length)
    {
    }

    template <class Ptr>
    inline BufferRef::BufferRef(Ptr *data)
      : m_data { reinterpret_cast<void *>(data) }, m_length { sizeof(Ptr) }
    {
    }

    inline BufferRef::BufferRef(Block const &block): m_data { block.addr }, m_length { block.size }
    {
    }

    inline BufferRef::BufferRef(std::string const &string)
      : m_data { reinterpret_cast<void *>(const_cast<char *>(string.data())) }, m_length { string.length() }
    {
    }

    template <class Ref>
    requires std::is_reference_v<Ref>
    auto inline BufferRef::as_ref() noexcept -> Ref
    {
        using base_type    = std::remove_reference_t<Ref>;
        using pointer_type = std::add_pointer_t<base_type>;

        return (*reinterpret_cast<pointer_type>(m_data));
    }

    template <class Ptr>
    requires std::is_pointer_v<Ptr>
    auto inline BufferRef::as_ptr() noexcept -> Ptr
    {
        return (reinterpret_cast<Ptr>(m_data));
    }

    auto inline BufferRef::length() const noexcept -> usize
    {
        return (m_length);
    }

    auto inline BufferRef::slice(usize length) const -> BufferRef
    {
        return (BufferRef { m_data, length });
    }
}  // namespace memory
