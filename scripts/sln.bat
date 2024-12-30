:: Предназначен для генерации решения "Visual Studio 17 2022".

@echo off

set SCRIPT_DIR=%~dp0

:: меняем путь на папку со скриптом, если ещё не в ней
if cd neq %SCRIPT_DIR% (
    cd /d %SCRIPT_DIR%
)

cd ..

if not exist build (
    mkdir build
) else (
    echo Cleaning build dir ...
	erase /q build
	for /d %%p in (build\*.* build\.vs) do (
		rmdir "%%p" /s /q
	)
)

cd build

cmake ..\src -G "Visual Studio 17 2022" -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=..\src\cmake\conan_provider.cmake -DCMAKE_BUILD_TYPE=Debug

if %ERRORLEVEL% neq 0 (
    echo [ERROR] cmake failed to generate solution. Please check cmake log.
    pause
    exit /b 1
)

start NavServer.sln
