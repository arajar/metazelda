-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------

newoption 
{
	trigger = "to",
	value   = "path",
	description = "Set the output location for the generated files"
}
  
newoption 
{
	trigger = "arch",
	value   = "String",
	description = "type of platform"
}

-- add a new flag in order to support the v110_xp toolset to support running apps on windows xp
premake.api.addAllowed("flags", "XpCompat") 
premake.api.addAllowed("flags", "LinkerWarningAsError")

premake.extensions.w = {}

function premake.extensions.w.customPlatformToolset(base, cfg)
	local p = cfg.flags.XpCompat and "v110_xp" or "v110"
	if _ACTION > "vs2012" then
		p = cfg.flags.XpCompat and "v120_xp" or "v120"
	end	
	if _ACTION > "vs2010" then
		_p(2,'<PlatformToolset>%s</PlatformToolset>', p)
	end
end

function premake.extensions.w.customProgramDataBaseFileName(base, cfg)
	if cfg.flags.Symbols and cfg.debugformat ~= "c7" then
		if cfg.kind == premake.CONSOLEAPP then
			local filename = cfg.buildtarget.basename
			_p(3,'<ProgramDataBaseFileName>$(IntDir)vc$(PlatformToolsetVersion).pdb</ProgramDataBaseFileName>')
		else
			base(cfg)
		end	
	end
end


function premake.extensions.w.customTreatLinkerWarningAsErrors(base, cfg)
	if cfg.flags.FatalWarnings and cfg.flags.LinkerWarningAsError then
		local el = iif(cfg.kind == premake.STATICLIB, "Lib", "Linker")
		_p(3,'<Treat%sWarningAsErrors>true</Treat%sWarningAsErrors>', el, el)
	end
end 

--todo add flag for custom database ???

premake.override(premake.vstudio.vc2010, 'platformToolset',  premake.extensions.w.customPlatformToolset)
premake.override(premake.vstudio.vc2010, 'programDataBaseFileName',  premake.extensions.w.customProgramDataBaseFileName)
premake.override(premake.vstudio.vc2010, 'treatLinkerWarningAsErrors',  premake.extensions.w.customTreatLinkerWarningAsErrors)

-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------

function GetPathFromPlatform(action)
	local act = action or _ACTION
	return act .."/" .._OPTIONS['arch']
end

-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------

--debugFlags = {"Symbols", "StaticRuntime"}
--releaseFlags = {"Symbols", "StaticRuntime"}

debugFlags = {"Symbols"}
releaseFlags = {"Symbols"}

commonDefines = { "_SCL_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}
 
debugDefines = {"_DEBUG"}
releaseDefines = {"NDEBUG", "_SECURE_SCL=0", "_SECURE_SCL_THROWS=0"}


-- for all release configurations including with exceptions
configuration "Release*"
	defines { releaseDefines, commonDefines }
	flags{ releaseFlags }
	optimize "Speed"

-- for all debug configurations including with exceptions
configuration "Debug*"
	defines { debugDefines, commonDefines }
	flags { debugFlags }	

--configuration "Release"
--	flags { "NoRTTI", "NoExceptions"}
	
--configuration "Debug"
--	flags { "NoRTTI", "NoExceptions"}
	
-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------

function addCommonConfig()

	language "C++"

	location ( _OPTIONS["to"] )
	
	configurations { "Debug", "Release" }

	--flags {"FatalWarnings", "NoMinimalRebuild" } 
		
	flags { "MultiProcessorCompile" }
	defines { "_WIN32_WINNT=0x0501"} -- needed for boost 
	buildoptions { "/wd4530"} -- silence warning about unwind semantics not enabled !!!
	
	if (_OPTIONS['arch'] == "x64") then
		architecture "x64"
	end	
end

-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------
