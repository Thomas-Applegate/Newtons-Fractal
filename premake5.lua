workspace "newtons-fractal"
	configurations {"debug", "release"}
project "newtons-fractal"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	targetname "newtons-fractal"
	files {"*.cpp", "*.h", "imgui/*.cpp", "imgui/*.h", "imgui/backends/imgui_impl_glfw.*",
		"imgui/backends/imgui_impl_opengl3.*","imgui/misc/cpp/imgui_stdlib.*",
		"glad/*"}
	includedirs {"imgui/", "imgui/backends", "glad/"}
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
