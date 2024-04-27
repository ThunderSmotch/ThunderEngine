project "Games"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++latest"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp", "src/**.cppm", "src/**.ixx" }

   includedirs
   {
      "src",
	  "../vendor/glad/include",
	  "../vendor/glfw/include",
	  "../vendor/",
	  
	  -- Include ThunderLib
	  "../ThunderLib/src"
   }

   links
   {
      "ThunderLib",
   }

   targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
   objdir ("../bin/int/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"