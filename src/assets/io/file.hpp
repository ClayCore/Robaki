/** @file file.hpp */

#pragma once

// module includes
#include "util/util.hpp"


// c++ includes
#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <utility>


namespace assets::io::file
{
    enum class FileType
    {
        Audio,
        BinCode,
        Config,
        Map,
        Mesh,
        Midi,
        Palette,
        PlainText,
        Sprite,
        Texture,
        Tracker,
    };

    namespace details
    {
        using namespace util::literals;

        // clang-format off
        static const std::map<FileType, std::vector<u32>> FILETYPES_ASSOC = {
            { FileType::Audio,     {"wav"_sh, "mp3"_sh, "ogg"_sh  }},
            { FileType::BinCode,   {"bin"_sh, "dat"_sh            }},
            { FileType::Config,    {"cfg"_sh,                     }},
            { FileType::Map,       {"map"_sh,                     }},
            { FileType::Mesh,      {"msh"_sh,                     }},
            { FileType::Midi,      {"mid"_sh,                     }},
            { FileType::Palette,   {"pal"_sh,                     }},
            { FileType::PlainText, {"txt"_sh,                     }},
            { FileType::Sprite,    {"spr"_sh,                     }},
            { FileType::Texture,   {"png"_sh, "jpg"_sh, "tiff"_sh }},
            { FileType::Tracker,   {"s3m"_sh, "mod"_sh, "xm"_sh   }}
        };
        // clang-format on
    }  // namespace details

    class File
    {
    private:
        using Handle = std::pair<std::fstream, std::string>;

        std::string m_path {};
        Handle m_handle {};
        FileType m_type { FileType::PlainText };

    public:
        File()             = default;
        File(File const &) = delete;
        File(File &&)      = delete;

        auto operator=(File const &) -> File & = delete;
        auto operator=(File &&) -> File      & = delete;

        explicit File(std::string const &path);
        explicit File(std::string_view path);

        ~File();

        [[nodiscard]] auto path() const -> std::string const &;

        [[nodiscard]] auto path() -> std::string;

        [[nodiscard]] auto handle() -> Handle &;

        [[nodiscard]] auto handle() const -> Handle const &;

        [[nodiscard]] auto type() const -> FileType;

        [[nodiscard]] auto type_from_ext() const -> FileType;

        auto open() -> void;

        auto read() -> void;

        auto close() -> void;

        auto with_path(std::string_view path) -> File &;
    };
}  // namespace assets::io::file
