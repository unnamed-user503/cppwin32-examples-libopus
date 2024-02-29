@echo off
chcp 65001

:bootstrap

    setlocal
    pushd "%~dp0\..\"

    call :main

    popd
    endlocal

    pause
    exit /b


:main
    call ".\Vendor\Premake5\Bin\premake5.exe" --file=Build.lua vs2022
    exit /b
