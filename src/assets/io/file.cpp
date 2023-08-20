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

    auto File::path() const -> std::string const &
    {
        return (m_path);
    }

    auto File::path() -> std::string
    {
        return (m_path);
    }

    auto File::handle() -> std::fstream &
    {
        return (m_handle);
    }

    auto File::handle() const -> std::fstream const &
    {
        return (m_handle);
    }

    auto File::type() const -> FileType
    {
        return (m_type);
    }

    auto File::mode() const -> Mode
    {
        return (m_mode);
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

    auto File::set_mode(Mode mode) -> void
    {
        m_mode = mode;
    }

    auto File::open() -> void
    {
        SCOPE_FAIL
        {
            // TODO: logging
            fmt::print(stderr, "[IO/ERROR]:\n\t\'{}\' could not be opened.", m_path);
        };

        switch (m_mode) {
        case Mode::Read: {
            m_handle.open(m_path, std::ios::in);
        } break;
        case Mode::Write: {
            m_handle.open(m_path, std::ios::out);
        } break;
        case Mode::Append: {
            m_handle.open(m_path, std::ios::app);
        }
        case Mode::ReadBin: {
            m_handle.open(m_path, std::ios::in | std::ios::binary);
        } break;
        case Mode::WriteBin: {
            m_handle.open(m_path, std::ios::out | std::ios::binary);
        } break;
        case Mode::AppendBin: {
            m_handle.open(m_path, std::ios::app | std::ios::binary);
        }
        }
    }

    auto File::read_lines() -> std::vector<std::string>
    {
        std::vector<std::string> result {};

        std::string line {};
        while (std::getline(m_handle, line)) {
            result.push_back(line);
        }

        return (result);
    }

    auto File::close() -> void
    {
        m_handle.close();
    }

    auto File::with_path(std::string_view path) -> File &
    {
        std::filesystem::path fs_path = { path };

        m_path   = path;
        m_type   = this->type_from_ext();
        m_handle = std::fstream { m_path };

        return (*this);
    }
}  // namespace assets::io::file
