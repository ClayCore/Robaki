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

    enum class Mode
    {
        Read,
        Write,
        Append,
        ReadBin,
        WriteBin,
        AppendBin
    };

    namespace details
    {
        using namespace util::literals;

        // clang-format off
        std::map<FileType, std::vector<u32>> static const FILETYPES_ASSOC = {
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
        std::string m_path {};
        std::fstream m_handle {};

        FileType m_type { FileType::PlainText };
        Mode m_mode { Mode::Read };

    public:
        File()             = default;
        File(File const &) = delete;
        File(File &&)      = delete;

        auto operator=(File const &) -> File & = delete;
        auto operator=(File &&) -> File      & = delete;

        explicit File(std::string const &path);
        explicit File(std::string_view path);

        [[nodiscard]] auto path() const -> std::string const &;

        [[nodiscard]] auto path() -> std::string;

        [[nodiscard]] auto handle() -> std::fstream &;

        [[nodiscard]] auto handle() const -> std::fstream const &;

        [[nodiscard]] auto type() const -> FileType;

        [[nodiscard]] auto mode() const -> Mode;

        [[nodiscard]] auto type_from_ext() const -> FileType;

        auto set_mode(Mode mode) -> void;

        auto open() -> void;

        auto read_lines() -> std::vector<std::string>;

        auto close() -> void;

        auto with_path(std::string_view path) -> File &;
    };
}  // namespace assets::io::file
