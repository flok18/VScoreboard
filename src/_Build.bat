@echo off

if not "%INCLUDE%"=="" goto :IncludesDefined
Set INCLUDE=C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Tools\MSVC\14.10.25017\include
Set INCLUDE=%INCLUDE%;C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\um
Set INCLUDE=%INCLUDE%;C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\shared
Set INCLUDE=%INCLUDE%;C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\ucrt
:IncludesDefined

if not "%LIB%"=="" goto :LibsDefined
Set LIB=C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Tools\MSVC\14.10.25017\lib\x64
Set LIB=%LIB%;C:\Program Files (x86)\Windows Kits\10\Lib\10.0.14393.0\um\x64
Set LIB=%LIB%;C:\Program Files (x86)\Windows Kits\10\Lib\10.0.14393.0\ucrt\x64
Set Path=%Path%;C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Tools\MSVC\14.10.25017\bin\HostX64\x64
Set Path=%Path%;C:\Program Files (x86)\Windows Kits\10\bin\x64;
:LibsDefined


Set ObjDir=..\obj
Set BinDir=..\bin


if not exist "%ObjDir%" md "%ObjDir%"
if not exist "%BinDir%" md "%BinDir%"

RC.exe /fo"%ObjDir%\Resource.res" /nologo Resource.rc
CVTRES.exe /MACHINE:X64 /NOLOGO /OUT:"%ObjDir%\Resource.obj" %ObjDir%\Resource.res
Set ObjList="%ObjDir%\Resource.obj"

for %%I in (*.c) do Call :AddObj %%I

Set ObjList=%ObjList% kernel32.lib gdi32.lib user32.lib
Set ObjList=%ObjList% ole32.lib comdlg32.lib winmm.lib

LINK.exe /nologo /MANIFEST:NO %ObjList% /Subsystem:windows /OUT:"%BinDir%\Scoreboard.exe"


del %ObjDir%\*.obj
del %ObjDir%\*.res

Pause
Exit


:AddObj
CL.exe /c /nologo /W4 /Gd /wd4100 /we4013 /O2 "%~1" /Fo"%ObjDir%\%~n1.obj" 
Set ObjList=%ObjList% "%ObjDir%\%~n1.obj"
Exit /B

