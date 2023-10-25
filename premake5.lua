-- premake5.lua

workspace "Banana"
    architecture "x64"
    configurations { "Debug", "Release" }
    staticruntime "off"

    defines {
      "MACRO_GLFW",
      "MACRO_OPENGL"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Banana/vendor/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/Banana/vendor/GLAD/include/glad"
IncludeDir["IMGUI"] = "%{wks.location}/Banana/vendor/IMGUI"
IncludeDir["GLM"] = "%{wks.location}/Banana/vendor/GLM/glm"
IncludeDir["STB"] = "%{wks.location}/Banana/vendor/STB"
IncludeDir["MSDFATLASGEN"] =  "%{wks.location}/Banana/vendor/MSDF-ATLAS-GEN/msdf-atlas-gen" 
IncludeDir["MSDF"] =            "%{wks.location}/Banana/vendor/MSDF-ATLAS-GEN/msdfgen"
IncludeDir["MSDFINC"] =        "%{wks.location}/Banana/vendor/MSDF-ATLAS-GEN/msdfgen/include"
IncludeDir["MINIAUDIO"] = "%{wks.location}/Banana/vendor/MINIAUDIO/"

group "dependencies"
include "Banana/vendor/GLFW"
include "Banana/vendor/GLAD"
include "Banana/vendor/IMGUI"
include "Banana/vendor/MSDF-ATLAS-GEN/msdfgen/freetype"
include "Banana/vendor/MSDF-ATLAS-GEN/msdfgen"
include "Banana/vendor/MSDF-ATLAS-GEN"
group ""

group "engine"
include "Banana/"
group ""

group "game"
include "Sandbox/"
group ""
