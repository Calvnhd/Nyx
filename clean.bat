@echo off

call %~dp0\vars.bat

rmdir /s /q "%PROJECT_DIR%\Intermediate"
rmdir /s /q "%PROJECT_DIR%\Binaries"
del "%PROJECT_DIR%\%PROJECT%.sln"
