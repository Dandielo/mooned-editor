project "TestEditor"
    kind "ConsoleApp"

    qtmodules { "core", "gui", "widgets", "opengl" }

    files {
        "public/**.h",
        "public/**.inl",
        "private/**.cpp",
        "private/**.h",
        "private/**.ui",
        "private/**.inl",
        "natvis/**.natvis",
    }

    includedirs {
        "public",
        "private",
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

    flags { "MultiProcessorCompile" }

    vpaths {
        [""] = { "premake5.lua" },
        ["Source/*"] = { "private/**", "public/meditor/**" },
        ["Natvis"] = { "natvis/**.natvis" },
        ["Generated/Moc/*"] = { "../../tmp/obj/**.cpp" },
        ["Generated/Ui/*"] = { "../../tmp/obj/**.h" }
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
