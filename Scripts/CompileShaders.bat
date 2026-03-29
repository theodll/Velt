@echo off
setlocal enabledelayedexpansion

REM --- Config ---
set DXC=%VULKAN_SDK%\Bin\dxc.exe

set ROOT=%~dp0
set SHADER_SRC_DIR=%ROOT%..\Assets\Shader
set SHADER_BIN_DIR=%ROOT%CompiledShaders

REM >>> DEIN TARGET (angepasst wie gewünscht) <<<
set TARGET_SHADER_DIR=%ROOT%..\out\build\x64-Debug\bin\Assets\Shader

REM Create dirs
if not exist "%SHADER_BIN_DIR%" mkdir "%SHADER_BIN_DIR%"
if not exist "%TARGET_SHADER_DIR%" mkdir "%TARGET_SHADER_DIR%"

REM --- Shader List ---
set SHADERS=^
basic_vertex_shader.hlsl vs_6_0 ^
basic_fragment_shader.hlsl ps_6_0 ^
gbuffer_vertex.hlsl vs_6_0 ^
gbuffer_pixel.hlsl ps_6_0 ^
deffered_pbr_vertex.hlsl vs_6_0 ^
deffered_pbr_pixel.hlsl ps_6_0

echo =============================
echo Compiling shaders...
echo =============================

REM --- Compile Loop ---
for %%i in (%SHADERS%) do (
    if defined FILE (
        set PROFILE=%%i

        set SRC=%SHADER_SRC_DIR%\!FILE!
        set OUT=%SHADER_BIN_DIR%\!FILE!.spv

        echo.
        echo [DXC] !FILE!

        "%DXC%" ^
            -T !PROFILE! ^
            -E main ^
            -spirv ^
            -fspv-target-env=vulkan1.3 ^
            "!SRC!" ^
            -Fo "!OUT!"

        if errorlevel 1 (
            echo ERROR compiling !FILE!
            exit /b 1
        )

        REM --- Copy direkt nach Build Output ---
        copy /Y "!OUT!" "%TARGET_SHADER_DIR%\" >nul

        if errorlevel 1 (
            echo ERROR copying !FILE!.spv
            exit /b 1
        )

        set FILE=
    ) else (
        set FILE=%%i
    )
)

echo.
echo =============================
echo All shaders compiled + deployed
echo =============================

exit /b 0