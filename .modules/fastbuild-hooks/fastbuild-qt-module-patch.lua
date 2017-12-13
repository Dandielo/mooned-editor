if not premake.extensions.qt then
    print("Qt module not found for patching...")
end

--
-- Override the base premake.fileconfig.addconfig method in order to add our
-- custom build rules for special Qt files.
--
-- @param base
--      The base method that we must call.
-- @param fcfg
--      The file configuration object.
-- @param cfg
--      The current configuration that we're adding to the file configuration.
--
function premake.extensions.qt.customAddFileConfig(base, fcfg, cfg)

    -- call the base method to add the file config
    base(fcfg, cfg)

    -- do nothing else if Qt is not enabled
    if cfg.qtenabled ~= true then
        return
    end

    -- get the current config
    local config = premake.fileconfig.getconfig(fcfg, cfg)

    -- now filter the files, and depending on their type, add our
    -- custom build rules

    local qt = premake.extensions.qt

    -- ui files
    if qt.isUI(config.abspath) then
        qt.addUICustomBuildRule(config, cfg)

    -- resource files
    elseif qt.isQRC(config.abspath) then
        qt.addQRCCustomBuildRule(config, cfg)

    -- moc files
    elseif qt.needMOC(config.abspath) then
        qt.addMOCCustomBuildRule(config, cfg)

    end


    --[[ Premake5-FASTBuild module patch start ]]--

    -- the cpp files generated by the qrc tool can't use precompiled header, so
    -- if we have pch and the file is a Qt generated one, check if it's generated
    -- by qrc to disable pch for it.
    if config.abspath:find(qt.getGeneratedDir(cfg), 1, true) then

        -- the generated dir path might contain special pattern character, so escape them
        local pattern = path.wildcards(qt.getGeneratedDir(cfg))

        -- if it's a qrc generated file, disable pch
        if cfg.pchheader and config.abspath:find(pattern .. "/qrc_.+%.cpp") then
            config.flags.NoPCH = true
            config.generated = true
        end
        if config.abspath:find(pattern .. "/moc_.+%.cpp") then
            config.generated = true
        end
    end

    --[[ Premake5-FASTBuild module patch end ]]--
end


--
-- Adds the custom build for ui files.
--
-- @param fcfg
--    The config for a single file.
-- @param cfg
--    The config of the project ?
--
function premake.extensions.qt.addUICustomBuildRule(fcfg, cfg)

    local qt = premake.extensions.qt

    -- get the output file
    local output = qt.getGeneratedDir(cfg) .. "/ui_" .. fcfg.basename .. ".h"


    --[[ Premake5-FASTBuild module patch start ]]--

    if _ACTION == "fastbuild" then

        assert(not fcfg.fbcustombuild)

        fcfg.fbcustombuild = { }
        fcfg.fbcustombuild.message    = "Uic'ing " .. fcfg.name
        fcfg.fbcustombuild.executable = path.translate(fcfg.config.qtbinpath .. "/uic.exe")

        local args = { "-o \"%2\" \"%1\"" }

        -- if we have custom commands, add them
        if fcfg.config.qtuicargs then
            table.foreachi(fcfg.config.qtuicargs, function (arg)
                table.insert(args, " \"" .. arg .. "\"")
            end)
        end

        -- fcfg.fbcustombuild.input = fcfg.relpath
        fcfg.fbcustombuild.arguments = args
        fcfg.fbcustombuild.output = output

        fcfg.buildcommands = {
            fcfg.fbcustombuild
        }
        fcfg.buildoutputs = output

    else 

        output = path.getrelative(fcfg.project.location, output)

        -- build the command
        local command = fcfg.config.qtbinpath .. "/uic -o \"" .. output .. "\" \"" .. fcfg.relpath.. "\""

        -- if we have custom commands, add them
        if fcfg.config.qtuicargs then
            table.foreachi(fcfg.config.qtuicargs, function (arg)
                command = command .. " \"" .. arg .. "\""
            end)
        end

        -- add the custom build rule
        fcfg.buildmessage   = "Uic'ing " .. fcfg.name
        fcfg.buildcommands  = { command }
        fcfg.buildoutputs   = { output }

    end

    --[[ Premake5-FASTBuild module patch end ]]--


end

--
-- Adds the custom build for ui files.
--
-- @param fcfg
--      The config for a single file.
-- @param cfg
--      The config of the project ?
--
function premake.extensions.qt.addQRCCustomBuildRule(fcfg, cfg)

    local qt = premake.extensions.qt

    -- get the input and output files
    local output = qt.getGeneratedDir(cfg) .. "/qrc_" .. fcfg.basename .. ".cpp"


    --[[ Premake5-FASTBuild module patch start ]]--

    if _ACTION == "fastbuild" then
        assert(not fcfg.fbcustombuild)

        fcfg.fbcustombuild = { }
        fcfg.fbcustombuild.message    = "Rcc'ing " .. fcfg.name
        fcfg.fbcustombuild.executable = path.translate(fcfg.config.qtbinpath .. "/rcc.exe")

        local args = { "-name \"" .. fcfg.basename .. "\" -no-compress \"%1\" -o \"%2\"" }

        -- if we have custom commands, add them
        if fcfg.config.qtrccargs then
            table.foreachi(fcfg.config.qtrccargs, function (arg)
                table.insert(args, " \"" .. arg .. "\"")
            end)
        end

        -- fcfg.fbcustombuild.input = fcfg.relpath
        fcfg.fbcustombuild.arguments = args
        fcfg.fbcustombuild.output = output

        fcfg.buildcommands = {
            fcfg.fbcustombuild
        }
        fcfg.buildoutputs = output

    else 
        output = path.getrelative(fcfg.project.location, output)

        -- build the command
        local command = fcfg.config.qtbinpath .. "/rcc -name \"" .. fcfg.basename .. "\" -no-compress \"" .. fcfg.relpath .. "\" -o \"" .. output .. "\""

        -- if we have custom commands, add them
        if fcfg.config.qtrccargs then
            table.foreachi(fcfg.config.qtrccargs, function (arg)
                command = command .. " \"" .. arg .. "\""
            end)
        end

        -- get the files embedded on the qrc, to add them as input dependencies :
        -- if we edit a .qml embedded in the qrc, we want the qrc to re-build whenever
        -- we edit the qml file
        local inputs = qt.getQRCDependencies(fcfg)

        -- add the custom build rule
        fcfg.buildmessage   = "Rcc'ing " .. fcfg.name
        fcfg.buildcommands  = { command }
        fcfg.buildoutputs   = { output }
        if #inputs > 0 then
            fcfg.buildinputs = inputs
        end
    end

    --[[ Premake5-FASTBuild module patch end ]]--


end

--
-- Adds the custom build for a moc'able file.
--
-- @param fcfg
--      The config for a single file.
-- @param cfg
--      The config of the project ?
--
function premake.extensions.qt.addMOCCustomBuildRule(fcfg, cfg)

    local qt = premake.extensions.qt

    -- get the project's location (to make paths relative to it)
    local projectloc = fcfg.project.location

    -- create the output file name
    local output = qt.getGeneratedDir(cfg) .. "/moc_" .. fcfg.basename .. ".cpp"


    --[[ Premake5-FASTBuild module patch start ]]--

    if _ACTION == "fastbuild" and not fcfg.flags.ExcludeFromBuild then
        assert(not fcfg.fbcustombuild)

        fcfg.fbcustombuild = { }
        fcfg.fbcustombuild.message    = "Moc'ing " .. fcfg.name
        fcfg.fbcustombuild.executable = path.translate(fcfg.config.qtbinpath .. "/moc.exe")
        local args = { "\"%1\" -o \"%2\"" }

        -- if we have a precompiled header, prepend it
        if fcfg.config.pchheader then
            table.insert(args, " \"-b" .. fcfg.config.pchheader .. "\"")
        end

        if #fcfg.config.defines > 0 then
            table.foreachi(fcfg.config.defines, function (define)
                table.insert(args, " -D" .. define)
            end)
        end

        -- append the include directories to the command
        if #fcfg.config.includedirs > 0 then
            table.foreachi(fcfg.config.includedirs, function (include)
                table.insert(args, " -I\"" .. path.getrelative(projectloc, include) .. "\"")
            end)
        end

        -- if we have custom commands, add them
        if fcfg.config.qtmocargs then
            table.foreachi(fcfg.config.qtmocargs, function (arg)
                table.insert(args, " \"" .. arg .. "\"")
            end)
        end

        fcfg.fbcustombuild.arguments = args
        fcfg.fbcustombuild.output = output

        fcfg.buildcommands = {
            fcfg.fbcustombuild
        }
        fcfg.buildoutputs = output

    else 

        -- create the output file name
        local output = qt.getGeneratedDir(cfg) .. "/moc_" .. fcfg.basename .. ".cpp"

        -- create the moc command
        local command = fcfg.config.qtbinpath .. "/moc \"" .. fcfg.relpath .. "\""
        command = command .. " -o \"" .. path.getrelative(projectloc, output) .. "\""

        -- if we have a precompiled header, prepend it
        if fcfg.config.pchheader then
            command = command .. " -b \"" .. fcfg.config.pchheader .. "\""
        end

        -- now create the arguments
        local arguments = ""

        -- append the defines to the arguments
        if #fcfg.config.defines > 0 then
            table.foreachi(fcfg.config.defines, function (define)
                arguments = arguments .. " -D" .. define
            end)
        end

        -- append the include directories to the arguments
        if #fcfg.config.includedirs > 0 then
            table.foreachi(fcfg.config.includedirs, function (include)
                arguments = arguments .. " -I\"" .. path.getrelative(projectloc, include) .. "\""
            end)
        end

        -- if we have custom commands, add them
        if fcfg.config.qtmocargs then
            table.foreachi(fcfg.config.qtmocargs, function (arg)
                arguments = arguments .. " \"" .. arg .. "\""
            end)
        end

        -- handle the command line size limit
        command = qt.handleCommandLineSizeLimit(cfg, fcfg, command, arguments)

        -- add the custom build rule
        fcfg.buildmessage   = "Moc'ing " .. fcfg.name
        fcfg.buildcommands  = { command }
        fcfg.buildoutputs   = { output }

    end

    --[[ Premake5-FASTBuild module patch end ]]--


end