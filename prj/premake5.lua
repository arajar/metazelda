-------------------------------------------------------------------------------

include "../tools/premake/common/"

-------------------------------------------------------------------------------

solution "metazelda"
	math.randomseed(string.hash(solution().name))
	startproject "metazelda"
	addCommonConfig()

-------------------------------------------------------------------------------

	configuration "Debug"
		targetsuffix "-d"

-------------------------------------------------------------------------------

	local LIBS      = "../libs/"
	local SRC       = "../src/"

	local DUNGEONS	= LIBS.."dungeons/"

	local BUILD_DIR = "../build/".._OPTIONS["arch"].."/"
	local RELEASE_DIR = "../release/".._OPTIONS["arch"].."/"

-------------------------------------------------------------------------------

project "dungeons"
	targetdir(BUILD_DIR)
	kind "StaticLib"
	language "C++"

	files
	{
		DUNGEONS.."/src/**",
	}

	configuration "Debug"
		defines { "_DEBUG" }
		buildoptions { "/MDd" }

	configuration "Release"
		defines { "NDEBUG" }
		buildoptions { "/MD" }

-------------------------------------------------------------------------------

project "viewer"
	targetdir(RELEASE_DIR)
	kind "ConsoleApp"
	debugdir("$(TargetDir)")

	includedirs 
	{
		SRC,
		DUNGEONS,
	}

	files
	{
		SRC.."**",
	}

	libdirs 
	{ 
		BUILD_DIR,
	}

	links
	{
		"dungeons",
	}

	targetname( "viewer_" .. _OPTIONS["arch"])

	configuration "Debug"
		defines { "_DEBUG" }
		buildoptions { "/MDd" }

	configuration "Release"
		defines { "NDEBUG" }
		buildoptions { "/MD" }

---------------------------------------------------------------------------

