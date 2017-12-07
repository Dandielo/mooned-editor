require "qt"
require "fastbuild"
require "fastbuild-hooks"

local qt = premake.extensions.qt

fbcompilers { 
    {
        name = "win64_x64",
        system = "windows",
        architecture = "x86_64",
        toolset = "msc_v141",
        path = ".fbuild/compilers/win_x64_v141.bff"
    }
}

newoption { 
    trigger = "qtbasepath",
    description = "Provides the base path for a Qt version which will be used for the Editor project.",
    value = "PATH"
}

-- Get the defaultpath found by Qt the module or use the path given from the command line
qtbasepath = qt.defaultpath or _OPTIONS["qtbasepath"]

workspace "MEditor"
    platforms { "x64" }
    configurations { "Debug", "Release" }

    -- If fastbuild is used then used the c7 debug format to enable obj file caching.
    if _ACTION == "fastbuild" then
        debugformat "c7"
    end

    architecture "x64"
    toolset "v141"

    targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
    objdir "tmp/obj/%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}"

    -- Enable QT support
    qt.enable()
    qtpath(qtbasepath)
    qtprefix "Qt5"

    -- Configuration specific stuff
    filter { "Debug" }
        symbols "On"
        targetsuffix "d"
        qtsuffix "d"

    -- Include projects in the 'src' directory
    include "src"
