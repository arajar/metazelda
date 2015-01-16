@echo off 
SET MAKETOOL=..\tools\premake\release\premake5.exe

rem %MAKETOOL% --arch=x86 --to=vs2013\x86 vs2013
%MAKETOOL% --arch=x64 --to=vs2013\x64 vs2013

pause