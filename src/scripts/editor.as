
[exporter]
class ExampleLuaExporter : CExporter
{
    ExampleLuaExporter()
    {
        print("ExampleLuaExporter()");
    }

    void OnExportGraph(IGraph@ graph, const string &in name)
    {
        writeln("-- Generated lua file from graph: " + graph.name + " " + name);
        flush(graph.name + ".lua");
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

    array<string> AvailableExporters()
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
