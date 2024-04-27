-- premake5.lua
workspace "Thunder"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Games"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus", "/MP" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Dependencies"
   include "vendor/imgui"
   include "vendor/glad"
   include "vendor/miniaudio"
   include "vendor/stb"
   --include "vendor/glfw" -- Maybe compile GLFW in the future
group ""

---group "ThunderLib"
include "ThunderLib/Build-ThunderLib.lua"
---group ""

include "Games/Build-Games.lua"

-- Workaround bug related to modules with the same name
require("vstudio")
premake.override(premake.vstudio.vc2010.elements, "clCompile", function(base, prj)
    local m = premake.vstudio.vc2010
    local calls = base(prj)

    if premake.project.iscpp(prj) then
		table.insertafter(calls, premake.xmlDeclaration,  function()
			premake.w('<ModuleDependenciesFile>$(IntDir)\\%%(RelativeDir)</ModuleDependenciesFile>')
			premake.w('<ModuleOutputFile>$(IntDir)\\%%(RelativeDir)</ModuleOutputFile>')
			premake.w('<ObjectFileName>$(IntDir)\\%%(RelativeDir)</ObjectFileName>')
		end)
    end

    return calls
end)

vpaths { }