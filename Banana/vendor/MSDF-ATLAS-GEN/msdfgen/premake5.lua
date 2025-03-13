project "msdfgen"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/target/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin/obj/" .. outputdir .. "/%{prj.name}")

	files {
		"core/**.h",
		"core/**.hpp",
		"core/**.cpp",
		"ext/**.h",
		"ext/**.hpp",
		"ext/**.cpp",
		"lib/**.cpp",
		"include/**.h"
	}

	includedirs {
		"include",
		"freetype/include"
	}

	defines {
		"MSDFGEN_USE_CPP11"
	}

	links {
		"freetype",
	}

	filter "system:windows"
		systemversion "latest"

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