-- Make sure all generated projects end up in the same location (easier to cleanup?)
local root_directory = os.getcwd()

-- Override the global 'project' function so we can set the location behind the scenes 
premake.override(_G, "project", function(base, name)
    local result = base(name)

    location(path.join(root_directory, "tmp/projects"))
    return result
end)
