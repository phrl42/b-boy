project "Banana"
    cppdialect "C++17"
    kind "StaticLib"
    language "C++"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/target/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin/obj/" .. outputdir .. "/%{prj.name}")

    pchheader "incs.h"
    pchsource "%{wks.location}/Banana/src/incs.cpp"
  
    files {
      "src/**.cpp",
      "src/**.h",
      "src/**.hpp"
    }

    links {

    }
    
    includedirs {
      "%{IncludeDir.GLFW}",
      "%{IncludeDir.GLAD}",
      "%{IncludeDir.IMGUI}",
      "%{IncludeDir.GLM}",
      "%{IncludeDir.STB}",
      "%{IncludeDir.MSDFATLASGEN}",
      "%{IncludeDir.MSDF}",
      "%{IncludeDir.MSDFINC}",
      "%{IncludeDir.MINIAUDIO}",
      "%{wks.location}/Banana",
      "%{wks.location}/Banana/src",
      "%{wks.location}/Banana/src/generic",
      "%{wks.location}/Banana/vendor/GLAD/include",

      "%{wks.location}/Sandbox/",
      "%{wks.location}/Sandbox/src"
    }

    filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

  filter "configurations:Debug"
    defines "BUILD_DEBUG"
    symbols "On"
    buildoptions { "" }
    linkoptions { "" }

  filter "configurations:Release"
    defines "BUILD_RELEASE"
    optimize "On"
    buildoptions { "" }
    linkoptions { "" }
