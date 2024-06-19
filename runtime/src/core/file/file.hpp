#pragma once

#include<filesystem>
#include<fstream>
#include<unordered_map>

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/serialization.hpp"

[[nodiscard]]
ACDA_API auto to_filepath(const std::string& string) -> std::filesystem::path;

[[nodiscard]]
ACDA_API auto to_filepath(const char* str) -> std::filesystem::path;

[[nodiscard]]
ACDA_API auto load_text(const std::filesystem::path& filepath) -> std::string;

struct File
{
public:
    ACDA_EXCEPTION(LoadFailed);
    ACDA_EXCEPTION(SaveFailed);
    ACDA_EXCEPTION(SectionNotFound);

    using section_type = Serialization::buffer_type;
    using section_storage_type = std::unordered_map<std::string, section_type>;

    using self_type = File;
public:

    static auto create_ifstream() -> std::ifstream;
    static auto create_ifstream(const std::filesystem::path& filepath) -> std::ifstream;
    static auto create_ofstream() -> std::ofstream;
    static auto create_ofstream(const std::filesystem::path& filepath) -> std::ofstream;

    File(const std::filesystem::path& filepath);
    File(const self_type&) = default;
    File(self_type&&) = default;
    ~File();

    auto operator=(const self_type&)->self_type & = default;
    auto operator=(self_type&&)->self_type & = default;

    /// @brief Load file from disk
    /// 
    /// @throw File::LoadFailed if failed to load
    auto load() -> self_type&;

    /// @brief Save file to disk
    ///
    /// @throw File::LoadFailed if failed to load
    auto save() -> self_type&;

    [[nodiscard]]
    auto get_section_or_create(const std::string& section_name) -> section_type&;

    [[nodiscard]]
    auto get_section(const std::string& section_name) -> section_type&;
    [[nodiscard]]
    auto get_section(const std::string& section_name) const -> const section_type&;

    auto has_section(const std::string& section_name) const -> bool;

    auto erase_section(const std::string& section_name) -> self_type&;

private:
    std::filesystem::path _filepath{};
    section_storage_type _section_storage{};
};
