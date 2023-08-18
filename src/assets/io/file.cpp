/** @file file.cpp */

// module includes
#include "file.hpp"

namespace assets::io::file
{
    File::File(std::string const &path)
    {
        this->with_path(path);
    }

    File::File(std::string_view path)
    {
        this->with_path(path);
    }

    File::~File()
    {
        m_handle.first.close();
    }

    auto File::path() const -> std::string const &
    {
        return (m_path);
    }

    auto File::path() -> std::string
    {
        return (m_path);
    }

    auto File::handle() -> Handle &
    {
        return (m_handle);
    }

    auto File::handle() const -> Handle const &
    {
        return (m_handle);
    }

    auto File::type() const -> FileType
    {
        return (m_type);
    }

    auto File::type_from_ext() const -> FileType
    {
        using namespace util::literals;

        auto ext  = m_path.substr(m_path.find_last_of('.') + 1);
        auto hash = util::string::hash_str(ext);
        for (const auto &[key, val] : details::FILETYPES_ASSOC) {
            if (std::find(val.cbegin(), val.cend(), hash) != val.cend()) {
                return (key);
            }
        }

        return (FileType::PlainText);
    }

    auto File::with_path(std::string_view path) -> File &
    {
        std::filesystem::path fs_path = { path };

        SCOPE_FAIL
        {
            fmt::print(stderr, "[IO/ERROR\n\t[{}] not found, empty or corrupt\n]", path);
        };

        m_path   = path;
        m_type   = this->type_from_ext();
        m_handle = std::make_pair(std::fstream(path), path);

        return (*this);
    }
}  // namespace assets::io::file
