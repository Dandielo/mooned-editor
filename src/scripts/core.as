abstract class CNode : INode
{
    CNode()
    {
        @_native_object = CNodeNative();
    }

    void OnCreate() { }
    void OnDestroy() { }

    // Properties
    string name
    {
        get { return _native_object.name; }
        set { _native_object.name = value; }
    }

    string description
    {
        get { return _native_object.description; }
        set { _native_object.description = value; }
    }

    // Methods
    void color(double r, double g, double b)
    {
        _native_object.color(r, g, b);
    }

    void move(int x, int y)
    {
        _native_object.move(x, y);
    }

    private CNodeNative@ _native_object;
}

abstract class CGraph : IGraph
{
    CGraph()
    {
        @_native_object = CGraphNative();
    }

    string name
    {
        get { return _native_object.name; }
    }

    string title
    {
        get { return _native_object.title; }
        set { _native_object.title = value; }
    }

    // Interface default implementations
    void OnCreate() { }
    void OnDestroy() { }
    void OnNodeAdded(INode@ node) { }
    void OnNodeRemoved(INode@ node) { }

    // Supported graph nodes
    array<string> NodeQueries() { return array<string>(); }

    // Forwards
    INode@ createNode(string type_name)
    {
        return _native_object.createNode(type_name);
    }

    private CGraphNative@ _native_object;
}

abstract class CProject : IProject
{
    CProject()
    {
        @_native_object = CProjectNative();
    }

    string name
    {
        get { return _native_object.name; }
    }

    void OnCreate() { }
    void OnLoad() { }
    void OnSave() { }
    void OnExport() { }
    void OnClose() { }

    // Available element classes
    array<string> AvailableElements() { return array<string>(); }

    private CProjectNative@ _native_object;
}

abstract class CExporter : IExporter
{
    CExporter()
    {
        @_native_object = CExporterNative();
    }

    void OnExportProject(IProject@) { }
    void OnExportGraph(IGraph@, const string &in) { }
    // void OnSerialize(INode@) { }

    void write(const string &in str) final
    {
        _native_object.write(str);
    }

    void writeln(const string &in str) final
    {
        _native_object.writeln(str);
    }

    void flush(const string &in file_name) final
    {
        _native_object.flush(file_name);
    }

    private CExporterNative@ _native_object;
}

abstract class CWorkspaceWindow : IWorkspaceWindow
{
    CWorkspaceWindow()
    {
        @_native_object = CWorkspaceWindowNative();
    }

    // Interface default implementations
    void OnCreate() { }
    void OnDestroy() { }

    private CWorkspaceWindowNative@ _native_object;
}
