-------------------------------------------------------------------------------

include "../tools/premake/common/"

-------------------------------------------------------------------------------

solution "metazelda"
	math.randomseed(string.hash(solution().name))
	startproject "viewer"
	addCommonConfig()

-------------------------------------------------------------------------------

	configuration "Debug"
		targetsuffix "-d"

-------------------------------------------------------------------------------

	local LIBS      = "../libs/"
	local SRC       = "../src/"

	local BUILD_DIR = "../build/".._OPTIONS["arch"].."/"
	local RELEASE_DIR = "../release/".._OPTIONS["arch"].."/"

-------------------------------------------------------------------------------

project "viewer"
	targetdir(RELEASE_DIR)
	kind "ConsoleApp"
	debugdir("$(TargetDir)")

	includedirs 
	{
		SRC,
	}

	files
	{
		SRC.."**",
	}

	libdirs 
	{ 
		BUILD_DIR,
	}

	targetname( "viewer_" .. _OPTIONS["arch"])

	configuration "Debug"
		defines { "_DEBUG" }
		buildoptions { "/MDd" }

	configuration "Release"
		defines { "NDEBUG" }
		buildoptions { "/MD" }

---------------------------------------------------------------------------

