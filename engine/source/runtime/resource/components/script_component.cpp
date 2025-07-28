#include "script_component.hpp"

#include <string>
#include <fstream>

#include "core/match.hpp"
#include "core/assert.hpp"

void Arcadia::ScriptComponent::LoadScript(const std::filesystem::path& filepath, const std::string& name)
{
    _Name = name;
    _Filepath = filepath;
    std::ifstream ifs(filepath);
    ACDA_ASSERT(ifs.is_open());
    _ScriptText.replace(
        _ScriptText.begin(),
        _ScriptText.end(),
        std::istreambuf_iterator<std::string::value_type>(ifs),
        std::istreambuf_iterator<std::string::value_type>()
    );
    ifs.close();
}

auto Arcadia::ScriptComponent::GetFilepath() const -> const std::filesystem::path&
{
    return _Filepath;
}

auto Arcadia::ScriptComponent::GetName() const -> const std::string&
{
    return _Name;
}

auto Arcadia::ScriptComponent::GetScriptText() const -> const std::string&
{
    return _ScriptText;
}

Arcadia::ScriptComponent::ScriptComponent(const nlohmann::json& json)
{
    LoadScript(json.at("filepath"), json.at("name"));
}

auto Arcadia::ScriptComponent::ToJson() const -> nlohmann::json
{
    return nlohmann::json{
        {"filepath", _Filepath.generic_string()},
        {"name", _Name}
    };
}

auto Arcadia::ScriptComponent::OnSnapshot() const -> std::unique_ptr<MementoType>
{
    std::unique_ptr<MementoType> memento_data_sptr = std::make_unique<MementoType>();
    memento_data_sptr->Filepath = _Filepath;
    memento_data_sptr->Name = _Name;
    memento_data_sptr->ScriptText = _ScriptText;
    return memento_data_sptr;
}

void Arcadia::ScriptComponent::OnRestore(const std::unique_ptr<MementoType>& memento_uptr)
{
    _Filepath = memento_uptr->Filepath;
    _Name = memento_uptr->Name;
    _ScriptText = memento_uptr->ScriptText;
}