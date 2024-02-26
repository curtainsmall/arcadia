#pragma once

#include<filesystem>
#include<fstream>
#include<unordered_map>

#include"core/base.hpp"
#include"core/exception.hpp"
#include"core/serialization.hpp"

namespace arcadia
{
    [[nodiscard]]
    ARCADIA_API auto to_filepath(const std::string& string) -> std::filesystem::path;

    [[nodiscard]]
    ARCADIA_API auto to_filepath(const char* str) -> std::filesystem::path;

    [[nodiscard]]
    ARCADIA_API auto load_text(const std::filesystem::path& filepath) -> std::string;

    struct ARCADIA_API file
    {
    public:
        ARCADIA_EXCEPTION(load_failed);
        ARCADIA_EXCEPTION(save_failed);
        ARCADIA_EXCEPTION(section_not_found);

        using section_type = arcadia::serialization::buffer_type;
        using section_umap_type = std::unordered_map<std::string, section_type>;

        using self_type = arcadia::file;
    public:

        static auto create_ifstream() -> std::ifstream;
        static auto create_ofstream() -> std::ofstream;

        file(const std::filesystem::path& filepath);
        file(const self_type&) = default;
        file(self_type&&) = default;
        ~file();

        auto operator=(const self_type&)->self_type & = default;
        auto operator=(self_type&&)->self_type & = default;

        /// @brief Load file from disk
        /// 
        /// @throw arcadia::file::load_failed if failed to load
        auto load() -> self_type&;

        /// @brief Save file to disk
        ///
        /// @throw arcadia::file::load_failed if failed to load
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
        section_umap_type _section_umap{};
    };

}
