project "MazeDrawer"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "Build/%{cfg.buildcfg}"

    targetdir("../bin/" .. outputdir .. "/%{prj.name}")
    targetdir("../bin-int/" .. outputdir .. "/%{prj.name}")
     
    files{
        "headers/**.h",
        "src/**.cpp",
        "Thirdparty/imgui/**.cpp",
        "Thirdparty/imgui/ImGuiDialog/**.cpp"
    }
    defines {"SFML_STATIC",
"SFML_USE_STATIC_STD_LIBS",}
    includedirs {
        "$(ProjectDir)Thirdparty/imgui/",
        "$(ProjectDir)Thirdparty/SFML/include/",
        "headers/",
    }
    libdirs {
        "$(ProjectDir)Thirdparty/SFML/lib/"
    }
    filter "configurations:Debug"
        defines { "DEBUG" }  
        runtime "Debug"
        links {
            "ogg",
            "openal32",
            "freetype",
            "sfml-audio-s-d",
            "sfml-graphics-s-d",
            "sfml-network-s-d",
            "sfml-system-s-d",
            "sfml-window-s-d",
            "winmm",
            "gdi32",
            "opengl32",
            "flac",
            "vorbisenc",
            "vorbisfile",
            "vorbis",
            "ws2_32"
        }

    filter "configurations:Release"  
        defines { "NDEBUG" }    
        runtime "Release"
        links {
            "ogg",
            "openal32",
            "freetype",
            "sfml-audio-s",
            "sfml-graphics-s",
            "sfml-network-s",
            "sfml-system-s",
            "sfml-window-s",
            "winmm",
            "gdi32",
            "opengl32",
            "flac",
            "vorbisenc",
            "vorbisfile",
            "vorbis",
            "ws2_32"
        }
        optimize "On" 
    