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
    ACDA_API auto ToFilepath(const std::string& string) -> std::filesystem::path;

    [[nodiscard]]
    ACDA_API auto ToFilepath(const char* str) -> std::filesystem::path;

    [[nodiscard]]
    ACDA_API auto LoadText(const std::filesystem::path& filepath) -> std::string;

    class File
    {
    public:
        ACDA_EXCEPTION(LoadFailed);
        ACDA_EXCEPTION(SaveFailed);
        ACDA_EXCEPTION(SectionNotFound);

        using SectionType = Serialization::BufferType;
        using SectionStorageType = std::unordered_map<std::string, SectionType>;

        using SelfType = File;
    public:

        static auto CreateIfstream() -> std::ifstream;
        static auto CreateIfstream(const std::filesystem::path& filepath) -> std::ifstream;
        static auto CreateOfstream() -> std::ofstream;
        static auto CreateOfstream(const std::filesystem::path& filepath) -> std::ofstream;

        File(const std::filesystem::path& filepath);
        File(const SelfType&) = default;
        File(SelfType&&) = default;
        ~File();

        auto operator=(const SelfType&)->SelfType & = default;
        auto operator=(SelfType&&)->SelfType & = default;

        /// @brief Load file from disk
        ///
        /// @throw File::LoadFailed if failed to load
        auto Load() -> SelfType&;

        /// @brief Save file to disk
        ///
        /// @throw File::LoadFailed if failed to load
        auto Save() -> SelfType&;

        [[nodiscard]]
        auto GetSectionOrCreate(const std::string& section_name) -> SectionType&;

        [[nodiscard]]
        auto GetSection(const std::string& section_name) -> SectionType&;
        [[nodiscard]]
        auto GetSection(const std::string& section_name) const -> const SectionType&;

        auto HasSection(const std::string& section_name) const -> bool;

        auto EraseSection(const std::string& section_name) -> SelfType&;

    private:
        std::filesystem::path _Filepath{};
        SectionStorageType _SectionStorage{};
    };
}