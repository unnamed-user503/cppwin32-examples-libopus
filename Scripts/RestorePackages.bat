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
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\msbuild.exe" Workspace.sln -t:restore -p:RestorePackagesConfig=true -p:RestorePackagesPath=".\Packages"
    exit /b
