/** @file buffer_ref.hpp */

#pragma once

// module includes
#include "block.hpp"
#include "util/util.hpp"
#include "utils.hpp"

namespace memory
{
    class BufferRef
    {
    private:
        void *m_data { nullptr };
        usize m_length { 0U };

    public:
        template <class Ptr>
        explicit inline BufferRef(Ptr *data, usize length);

        template <class Ptr>
        explicit inline BufferRef(Ptr const *m_data, usize length);

        template <class Ptr>
        explicit inline BufferRef(Ptr *m_data);

        explicit inline BufferRef(Block const &block);

        explicit inline BufferRef(std::string const &string);

        template <class Ref>
        requires std::is_reference_v<Ref>
        inline auto as_ref() noexcept -> Ref;

        template <class Ptr>
        requires std::is_pointer_v<Ptr>
        inline auto as_ptr() noexcept -> Ptr;

        [[nodiscard]] inline auto length() const noexcept -> usize;

        [[nodiscard]] inline auto slice(usize length) const -> BufferRef;
    };
}  // namespace memory
