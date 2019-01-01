project "TestEditor"
    kind "ConsoleApp"

    qtmodules { "core", "gui", "widgets", "opengl" }

    files {
        "**.cpp",
        "**.h",
        "**.ui",
        "**.inl",
        "**.natvis",
    }

    includedirs {
        ".",
        "../contrib/angelscript/include",
        "../contrib/angelscript/add_on/scriptbuilder",
        "../contrib/angelscript/add_on/scriptstdstring",
        "../contrib/angelscript/add_on/scriptarray",
    }

    links {
        "AngelScript",
        "AsScriptArray",
        "AsScriptBuilder",
        "AsScriptStdString",
    }


    -- Copy Qt libraries on post build steps
    filter { "Debug" }
        postbuildmessage "Copying Qt libraries..."
        postbuildcommands {
            "{COPY} %{qtbasepath}/bin/Qt5Cored.dll ../../bin/%{cfg.platform}/%{cfg.buildcfg}",
            "{COPY} %{qtbasepath}/bin/Qt5Widgetsd.dll ../../bin/%{cfg.platform}/%{cfg.buildcfg}",
            "{COPY} %{qtbasepath}/bin/Qt5Guid.dll ../../bin/%{cfg.platform}/%{cfg.buildcfg}",
            -- "{COPY} ../../src/scripts/ ../../bin/%{cfg.platform}/%{cfg.buildcfg}/"
        }

    filter { "Release" }
        postbuildmessage "Copying Qt libraries..."
        postbuildcommands {
            "{COPY} %{qtbasepath}/bin/Qt5Core.dll ../../bin/%{cfg.platform}/%{cfg.buildcfg}",
            "{COPY} %{qtbasepath}/bin/Qt5Widgets.dll ../../bin/%{cfg.platform}/%{cfg.buildcfg}",
            "{COPY} %{qtbasepath}/bin/Qt5Gui.dll ../../bin/%{cfg.platform}/%{cfg.buildcfg}",
            -- "{COPY} ../../src/scripts/ ../../bin/%{cfg.platform}/%{cfg.buildcfg}/"
        }
