
[exporter]
class ExampleLuaExporter : CExporter
{
    void OnSerialize(IProject@ proj)
    {
        print("Exporting project: " + proj.name);
    }
}

[project]
class ExampleProject : CProject
{
    void OnCreate() { }
    void OnSave() { }
    void OnLoad() { }
    void OnExport() { }
    void OnClose() { }

    array<string> AvailableElements() 
    { 
        return array<string> = { 
            "ExampleGraph"
        };
    }

    array<string> Exporters() 
    {
        return array<string> = { 
            "ExampleLuaExporter"
        };
    }
}

[editor]
class Editor : CWorkspaceWindow
{
    void OnCreate() 
    { 
        print("Workspace window created.");
    }
    
    void OnDestroy() 
    { 
        print("Workspace window destroyed.");
    }
}
