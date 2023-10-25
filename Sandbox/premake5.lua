project "Sandbox"
  cppdialect "C++17"
  kind  "ConsoleApp"
  language "C++"
  staticruntime "off"

    targetdir ("%{wks.location}/bin/target/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin/obj/" .. outputdir .. "/%{prj.name}")
    
  files {
    "src/**.cpp",
    "src/**.h",
    "src/**.hpp"
  }

  includedirs {
      "%{IncludeDir.GLFW}",
      "%{IncludeDir.GLAD}",
      "%{IncludeDir.IMGUI}",
      "%{IncludeDir.GLM}",
      "%{IncludeDir.STB}",
      "%{IncludeDir.MINIAUDIO}",
      "%{wks.location}/Banana",
      "%{wks.location}/Banana/src",
      "%{wks.location}/Banana/src/generic",

      "%{wks.location}/Sandbox",
      "%{wks.location}/Sandbox/src"
  }

  links { 
    "Banana",
    "GLAD",
    "IMGUI",
    "GLFW",
    "MSDF-ATLAS-GEN",
    "msdfgen",
    "freetype",
  }

  filter "configurations:Debug"
    defines "BUILD_DEBUG"
    symbols "On"

  filter "configurations:Release"
    defines "BUILD_RELEASE"
    optimize "On"
