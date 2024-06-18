#pragma once

#include<filesystem>
#include<fstream>
#include<unordered_map>

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/serialization.hpp"

[[nodiscard]]
ACDA_API auto ToFilepath(const std::string& string) -> std::filesystem::path;

[[nodiscard]]
ACDA_API auto ToFilepath(const char* str) -> std::filesystem::path;

[[nodiscard]]
ACDA_API auto LoadText(const std::filesystem::path& filepath) -> std::string;

struct File
{
public:
    ACDA_EXCEPTION(load_failed);
    ACDA_EXCEPTION(save_failed);
    ACDA_EXCEPTION(section_not_found);

    using section_type = Serialization::buffer_type;
    using section_storage_type = std::unordered_map<std::string, section_type>;

    using self_type = File;
public:

    static auto CreateIfstream() -> std::ifstream;
    static auto CreateIfstream(const std::filesystem::path& filepath) -> std::ifstream;
    static auto CreateOfstream() -> std::ofstream;
    static auto CreateOfstream(const std::filesystem::path& filepath) -> std::ofstream;

    File(const std::filesystem::path& filepath);
    File(const self_type&) = default;
    File(self_type&&) = default;
    ~File();

    auto operator=(const self_type&)->self_type & = default;
    auto operator=(self_type&&)->self_type & = default;

    /// @brief Load file from disk
    /// 
    /// @throw file::load_failed if failed to load
    auto Load() -> self_type&;

    /// @brief Save file to disk
    ///
    /// @throw file::load_failed if failed to load
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
