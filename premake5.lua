require "qt"
local qt = premake.extensions.qt

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

    architecture "x64"

    targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
    objdir "tmp/obj"

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
