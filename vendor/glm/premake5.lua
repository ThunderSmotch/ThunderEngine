project "glm"
	kind "StaticLib"
	language "C++"
    staticruntime "off"

	targetdir ("../../bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("../../bin/int/" .. OutputDir .. "/%{prj.name}")

    includedirs
    {
	  ".."
    }

	files
	{
		"**.h",
		"**.c",
		"**.hpp",
		"**.cpp",
		"**.cppm"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++latest"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

    filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"
