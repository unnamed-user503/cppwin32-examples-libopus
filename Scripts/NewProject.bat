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
    set /p PROJECT_NAME="PROJECT NAME: "

    if not exist "Projects\%PROJECT_NAME%" (
        robocopy "Templates\Projects\C++17" "Projects\%PROJECT_NAME%" /MIR /DCOPY:DAT /XF .gitkeep /XD .git
    )

    if exist "Projects\%PROJECT_NAME%\Build.lua" (
        start "" "Projects\%PROJECT_NAME%\Build.lua"
    )

    if exist "Build.lua" (
        start "" "Build.lua"
    )

    exit /b
