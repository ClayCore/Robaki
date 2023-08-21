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
        auto inline as_ref() noexcept -> Ref;

        template <class Ptr>
        requires std::is_pointer_v<Ptr>
        auto inline as_ptr() noexcept -> Ptr;

        [[nodiscard]] auto inline length() const noexcept -> usize;
        [[nodiscard]] auto inline slice(usize length) const -> BufferRef;
    };
}  // namespace memory
