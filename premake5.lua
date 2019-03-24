workspace "Skeletal"
	architecture "x64"
	startproject "Skeletal"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}"

include "Skeletal/vendor/GLFW"
include "Skeletal/vendor/Glad"


project "Skeletal"

	location "Skeletal"
	kind "ConsoleApp"
	language "C++"


	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/stb_image.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"Skeletal/vendor/GLFW/include",
		"Skeletal/vendor/Glad/include",
		"Skeletal/vendor/stb_image",
		"Skeletal/vendor/glm/include"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines { GLFW_INCLUDE_NONE }

		
	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "NDEBUG"
		optimize "On"