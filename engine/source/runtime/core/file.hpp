#pragma once

#include <filesystem>
#include <fstream>
#include <unordered_map>

#include "core/exception.hpp"
#include "core/serialization.hpp"
#include "platform/defines.hpp"

namespace Arcadia
{
    [[nodiscard]]
    ACDA_API auto ToFilepath(std::string_view string) -> std::filesystem::path;

    [[nodiscard]]
    ACDA_API auto ToFilepath(const char* str) -> std::filesystem::path;

    [[nodiscard]]
    ACDA_API auto LoadText(const std::filesystem::path& filepath) -> std::string;

    namespace Exceptions
    {
        ACDA_DEFINE_EXCEPTION(FileOpenFailed);
        ACDA_DEFINE_EXCEPTION(FileLoadFailed);
        ACDA_DEFINE_EXCEPTION(FileSaveFailed);
    }

    struct ACDA_API File
    {
    public:
        using SectionType = Serialization::BufferType;
        using SectionStorageType = std::unordered_map<std::string, SectionType>;

        using SelfType = File;
    public:

        File(const std::filesystem::path& filepath);
        File(const SelfType&) = default;
        File(SelfType&&) = default;
        ~File();

        auto operator=(const SelfType&)->SelfType & = default;
        auto operator=(SelfType&&)->SelfType & = default;

        auto Load() -> SelfType&;

        auto Save() -> SelfType&;

        [[nodiscard]]
        auto GetSectionOrCreate(std::string_view section_name) -> SectionType&;

        [[nodiscard]]
        auto GetSection(std::string_view section_name) -> SectionType&;
        [[nodiscard]]
        auto GetSection(std::string_view section_name) const -> const SectionType&;

        auto ContainsSection(std::string_view section_name) const -> bool;

        auto EraseSection(std::string_view section_name) -> SelfType&;

    private:
        std::filesystem::path _Filepath{};
        SectionStorageType _SectionStorage{};
    };
}