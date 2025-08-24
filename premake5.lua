workspace "newtwon-fractal"
	configurations {"debug", "release"}
project "newtwon-fractal"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	targetname "newtwon-fractal"
	files {"**.cpp", "**.h"}
	objdir "obj/%{cfg.buildcfg}"
	targetdir "bin/%{cfg.buildcfg}"
	
	filter "system:linux"
		links {"GL", "glfw"}
	
	filter "configurations:debug"
		defines { "DEBUG" }
		symbols "On"
		optimize "Debug"
	filter "configurations:release"
		defines { "NDEBUG" }
		optimize "On"
