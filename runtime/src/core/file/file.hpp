#pragma once

#include<filesystem>
#include<fstream>
#include<unordered_map>

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/serialization.hpp"

namespace Arcadia
{
    [[nodiscard]]
    ARCADIA_API auto ToFilepath(const std::string& string) -> std::filesystem::path;

    [[nodiscard]]
    ARCADIA_API auto ToFilepath(const char* str) -> std::filesystem::path;

    [[nodiscard]]
    ARCADIA_API auto LoadText(const std::filesystem::path& filepath) -> std::string;

    struct ARCADIA_API File
    {
    public:
        ARCADIA_EXCEPTION(load_failed);
        ARCADIA_EXCEPTION(save_failed);
        ARCADIA_EXCEPTION(section_not_found);

        using section_type = Arcadia::Serialization::buffer_type;
        using section_storage_type = std::unordered_map<std::string, section_type>;

        using self_type = Arcadia::File;
    public:

        static auto CreateIFstream() -> std::ifstream;
        static auto CreateIFstream(const std::filesystem::path& filepath) -> std::ifstream;
        static auto CreateOFstream() -> std::ofstream;
        static auto CreateOFstream(const std::filesystem::path& filepath) -> std::ofstream;

        File(const std::filesystem::path& filepath);
        File(const self_type&) = default;
        File(self_type&&) = default;
        ~File();

        auto operator=(const self_type&)->self_type & = default;
        auto operator=(self_type&&)->self_type & = default;

        /// @brief Load file from disk
        /// 
        /// @throw Arcadia::file::load_failed if failed to load
        auto Load() -> self_type&;

        /// @brief Save file to disk
        ///
        /// @throw Arcadia::file::load_failed if failed to load
        auto Save() -> self_type&;

        [[nodiscard]]
        auto GetSectionOrCreate(const std::string& section_name) -> section_type&;

        [[nodiscard]]
        auto GetSection(const std::string& section_name) -> section_type&;
        [[nodiscard]]
        auto GetSection(const std::string& section_name) const -> const section_type&;

        auto HasSection(const std::string& section_name) const -> bool;

        auto EraseSection(const std::string& section_name) -> self_type&;

    private:
        std::filesystem::path _Filepath{};
        section_storage_type _SectionStorage{};
    };

}
