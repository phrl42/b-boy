project "MSDF-ATLAS-GEN"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/target/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin/obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"msdf-atlas-gen/**.h",
    "msdf-atlas-gen/**.hpp",
    "msdf-atlas-gen/**.cpp"
	}

	includedirs
	{
		"msdf-atlas-gen",
		"msdfgen",
		"msdfgen/include"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	links
	{
    "freetype",
		"msdfgen"
	}

  filter "system:windows"
    systemversion "latest"
    
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"