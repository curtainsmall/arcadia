#include "pch.hpp"
#include "file.hpp"

#include<format>
#include<fstream>
#include<sstream>

ARCADIA_API auto arcadia::to_filepath(const std::string& string) -> std::filesystem::path
{
    std::filesystem::path path{ string };
    return path.make_preferred();
}

ARCADIA_API auto arcadia::to_filepath(const char* str) -> std::filesystem::path
{
    std::filesystem::path path{ str };
    return path.make_preferred();
}

ARCADIA_API auto arcadia::load_text(const std::filesystem::path& filepath) -> std::string
{
    std::ifstream ifs{ filepath };
    std::stringstream sstream{};

    sstream << ifs.rdbuf();

    return sstream.str();
}

auto arcadia::file::create_ifstream() -> std::ifstream
{
    std::ifstream ifs{};
    ifs.exceptions(std::ios::failbit);
    return ifs;
}

auto arcadia::file::create_ofstream() -> std::ofstream
{
    std::ofstream ofs{};
    ofs.exceptions(std::ios::failbit);
    return ofs;
}

arcadia::file::file(const std::filesystem::path& filepath):
    _filepath(filepath)
{}

arcadia::file::~file()
{
    for(auto& [section_name, section] : _section_umap)
    {
        section.clear();
    }
}

auto arcadia::file::load() -> self_type&
{
    std::ifstream ifs{ _filepath,std::ios_base::binary };
    if(ifs.fail())
    {
        throw load_failed(std::format("Cannot open file at", _filepath.generic_string()));
    }
    ifs.exceptions(std::ios_base::badbit);

    // Section count
    std::size_t section_count{ 0 };
    ifs.read(reinterpret_cast<char*>(&section_count), sizeof(section_count));

    // For each section
    while(section_count-- > 0)
    {
        // Section name
        std::size_t len{ 0 };
        ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
        char* buf = static_cast<char*>(std::malloc(len * sizeof(len)));
        ifs.read(buf, len);
        std::string section_name{ buf,len };

        // Section
        ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
        section_type section{ len };
        ifs.read(reinterpret_cast<char*>(section.data()), section.size());
        _section_umap.insert_or_assign(section_name, section);

    }

    return *this;

}

auto arcadia::file::save() -> self_type&
{
    std::ofstream ofs{ _filepath, std::ios_base::binary };
    if(ofs.fail())
    {
        throw save_failed{ std::format("Cannot open file at {}",_filepath.generic_string()) };
    }
    ofs.exceptions(std::ios_base::badbit);

    // Section count
    auto section_count = _section_umap.size();
    ofs.write(reinterpret_cast<const char*>(&section_count), sizeof(section_count));

    // For each section
    for(const auto& [section_name, section] : _section_umap)
    {
        // Section name
        std::size_t len = section_name.size();
        ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
        ofs.write(section_name.c_str(), len);

        // Section
        len = section.size();
        ofs.write(reinterpret_cast<const char*>(len), sizeof(len));
        ofs.write(reinterpret_cast<const char*>(section.data()), len);
    }

    return *this;
}

auto arcadia::file::get_section_or_create(const std::string& section_name) -> section_type&
{
    if(_section_umap.contains(section_name))
    {
        _section_umap.insert_or_assign(section_name, section_type{});
    }
    return get_section(section_name);
}

auto arcadia::file::get_section(const std::string& section_name) -> section_type&
{
    try
    {
        return _section_umap.at(section_name);
    }
    catch(const std::out_of_range)
    {
        throw section_not_found{ std::format("Cannot find section named {}",section_name) };
    }
}

auto arcadia::file::get_section(const std::string& section_name) const -> const section_type&
{
    try
    {
        return _section_umap.at(section_name);
    }
    catch(const std::out_of_range)
    {
        throw section_not_found{ std::format("Cannot find section named {}",section_name) };
    }
}

auto arcadia::file::has_section(const std::string& section_name) const -> bool
{
    return _section_umap.contains(section_name);
}

auto arcadia::file::erase_section(const std::string& section_name) -> self_type&
{
    _section_umap.erase(section_name);
    return *this;
}

