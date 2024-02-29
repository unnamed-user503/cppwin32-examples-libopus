project "libopusfile"
    kind          "StaticLib" -- ConsoleApp WindowedApp SharedLib StaticLib
    language      "C"
    warnings      "off"
    staticruntime "off"
    characterset  "MBCS"

    targetdir ("%{wks.location}/Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir    ("%{wks.location}/Binaries-Intermediates/" .. OutputDir .. "/%{prj.name}")

    vspropertysettings
    {
        VcpkgTriplet = "x64-windows-static",
        VcpkgEnabled = "true",
        PublicIncludeDirectories = "include",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS", "_LIB", "WIN32", "WIN64", -- "OP_ENABLE_HTTP"
    }

    files
    {
        "src/internal.h",
        "src/winerrno.h",
        "src/http.c",
        "src/info.c",
        "src/internal.c",
        "src/opusfile.c",
        "src/stream.c",
        "src/wincerts.c",
    }

    excludes
    {
    }

    includedirs
    {
        ".", "include", "silk", "celt", "win32", "silk/fixed", "silk/float",
    }

    links
    {
        "libogg", "libopus",
    }
