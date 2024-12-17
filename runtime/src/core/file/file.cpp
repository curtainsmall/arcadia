#include "pch.hpp"

#include "file.hpp"

#include<format>
#include<fstream>
#include<sstream>

#include"core/assert.hpp"

ACDA_API auto Arcadia::ToFilepath(const std::string& string) -> std::filesystem::path
{
    std::filesystem::path path(string);
    return path.make_preferred();
}

ACDA_API auto Arcadia::ToFilepath(const char* str) -> std::filesystem::path
{
    std::filesystem::path path(str);
    return path.make_preferred();
}

ACDA_API auto Arcadia::LoadText(const std::filesystem::path& filepath) -> std::string
{
    std::ifstream ifs(filepath);
    std::stringstream sstream{};

    sstream << ifs.rdbuf();

    return sstream.str();
}

auto Arcadia::File::CreateIfstream() -> std::ifstream
{
    std::ifstream ifs{};
    ifs.exceptions(std::ios::failbit);
    return ifs;
}

auto Arcadia::File::CreateIfstream(const std::filesystem::path& filepath) -> std::ifstream
{
    auto ifs = CreateIfstream();
    ifs.open(filepath);
    return ifs;
}

auto Arcadia::File::CreateOfstream() -> std::ofstream
{
    std::ofstream ofs{};
    ofs.exceptions(std::ios::failbit);
    return ofs;
}

auto Arcadia::File::CreateOfstream(const std::filesystem::path& filepath) -> std::ofstream
{
    auto ofs = CreateOfstream();
    ofs.open(filepath);
    return ofs;
}

Arcadia::File::File(const std::filesystem::path& filepath) :
    _Filepath(filepath)
{}

Arcadia::File::~File()
{
    for(auto& [section_name, section] : _SectionStorage)
    {
        section.clear();
    }
}

auto Arcadia::File::Load() -> SelfType&
{
    std::ifstream ifs(_Filepath, std::ios_base::binary);
    if(ifs.fail())
    {
        throw LoadFailed(std::format("Cannot open file at", _Filepath.generic_string()));
    }
    ifs.exceptions(std::ios_base::badbit);

    // Section Count
    std::size_t section_count(0);
    ifs.read(reinterpret_cast<char*>(&section_count), sizeof(section_count));

    // For each section
    while(section_count-- > 0)
    {
        // Section name
        std::size_t len(0);
        ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
        char* buf = static_cast<char*>(std::malloc(len * sizeof(len)));
        ACDA_ASSERT(buf);
        ifs.read(buf, len);
        std::string section_name(buf, len);

        // Section
        ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
        SectionType section(len);
        ifs.read(reinterpret_cast<char*>(section.data()), section.size());
        _SectionStorage.insert_or_assign(section_name, section);
    }

    return *this;
}

auto Arcadia::File::Save() -> SelfType&
{
    std::ofstream ofs(_Filepath, std::ios_base::binary);
    if(ofs.fail())
    {
        throw SaveFailed(std::format("Cannot open file at {}", _Filepath.generic_string()));
    }
    ofs.exceptions(std::ios_base::badbit);

    // Section Count
    auto section_count = _SectionStorage.size();
    ofs.write(reinterpret_cast<const char*>(&section_count), sizeof(section_count));

    // For each section
    for(const auto& [section_name, section] : _SectionStorage)
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

auto Arcadia::File::GetSectionOrCreate(const std::string& section_name) -> SectionType&
{
    if(_SectionStorage.contains(section_name))
    {
        _SectionStorage.insert_or_assign(section_name, SectionType());
    }
    return GetSection(section_name);
}

auto Arcadia::File::GetSection(const std::string& section_name) -> SectionType&
{
    try
    {
        return _SectionStorage.at(section_name);
    }
    catch(const std::out_of_range)
    {
        throw SectionNotFound(std::format("Cannot find section named {}", section_name));
    }
}

auto Arcadia::File::GetSection(const std::string& section_name) const -> const SectionType&
{
    try
    {
        return _SectionStorage.at(section_name);
    }
    catch(const std::out_of_range)
    {
        throw SectionNotFound(std::format("Cannot find section named {}", section_name));
    }
}

auto Arcadia::File::HasSection(const std::string& section_name) const -> bool
{
    return _SectionStorage.contains(section_name);
}

auto Arcadia::File::EraseSection(const std::string& section_name) -> SelfType&
{
    _SectionStorage.erase(section_name);
    return *this;
}