set ROOTDIR=%~dp0
set ROOTDIR=%ROOTDIR:~0,-1%

set PROJECT=Nyx
set PROJECT_DIR=%ROOTDIR%
set UPROJECT_PATH=%PROJECT_DIR%\%PROJECT%.uproject

set UE50_DIR=C:\Program Files\Epic Games\UE_5.3
set UE5EDITOR_EXE=%UE50_DIR%\Engine\Binaries\Win64\UnrealEditor.exe
set BUILD_BAT=%UE50_DIR%\Engine\Build\BatchFiles\Build.bat