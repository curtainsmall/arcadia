workspace "Arcadia"
    architecture "x64" 
    configurations {"Debug", "Release"}
    location "build"

local outputdir = "%{cfg.buildcfg}-%{cfg.eystem}-%{cfg.architecture}"

project "Runtime"
    kind "StaticLib"
    language "C++"
    location "build/runtime"

    targetdir("bin/" .. outputdir .. "/%{prj.name}" )
    objdir("obj/" .. outputdir .. "/&{prj.name}")

    files {
        "%{prj.name}/src/**.hpp"
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "3rdparty/"
        "3rdparty/assimp/include"
        "3rdparty/glew/include"
        "3rdparty/glfw/include"
        "3rdparty/imgui"
        "3rdparty/nlohmann_json"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

    filter "configuration:Debug"
        defines "ARCADIA_DEBUG"
    
project "Editor"
    kind "ConsoleApp"
    language "C++"
    location "build/editor"

    targetdir("bin/" .. outputdir .. "/%{prj.name}" )
    objdir("obj/" .. outputdir .. "/&{prj.name}")

    files{
        "%{prj.name}/src/**.hpp"
        "%{prj.name}/src/**.cpp"
    }

    links {"Runtime"}

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

    filter "configuration:Debug"
        defines "ARCADIA_DEBUG"