abstract class CNode : INode 
{
    CNode() 
    {
        @m_Object = CNodeNative();
    }

    void OnCreate() { }
    void OnDestroy() { }
    
    // Properties
    string name 
    { 
        get { return m_Object.name; }
        set { m_Object.name = value; }
    }
    
    string description 
    { 
        get { return m_Object.description; }
        set { m_Object.description = value; }
    }

    // Methods
    void color(double r, double g, double b)
    {
        m_Object.color(r, g, b);
    }

    void move(int x, int y)
    {
        m_Object.move(x, y);
    }

    private CNodeNative@ m_Object;
}

abstract class CGraph : IGraph
{
    CGraph() 
    {
        @m_Object = CGraphNative();
    }

    string name 
    { 
        get { return m_Object.name; }
    }

    string title 
    { 
        get { return m_Object.title; }
        set { m_Object.title = value; }
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
        return m_Object.createNode(type_name);
    } 

    private CGraphNative@ m_Object;
}

abstract class CWorkspaceWindow : IWorkspaceWindow
{
    CWorkspaceWindow() 
    { 
        @m_Object = CWorkspaceWindowNative();
    }

    // Interface default implementations
    void OnCreate() { }
    void OnDestroy() { }

    private CWorkspaceWindowNative@ m_Object;
}
