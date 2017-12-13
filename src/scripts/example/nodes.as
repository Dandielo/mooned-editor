
[node, desc:Tutorial node, name:End]
class TutorialNodeIn : CNode 
{
    [type: in]
    prop<int> In;

    void OnCreate()
    {
        print(name + " node created.");
        color(0.0, 0.8, 0.3); //set the color to red
        _once = true;
    }

    void OnDestroy()
    {
        print(name + " node destroyed.");
    }

    void Update()
    {
        if (_once)
        {
            INode@ n = _graph.createNode("TutorialNodeOut");
            n.move(4700, 4900); // move to center
            _once = false;
        }
        else
        {
            int num = In;
            name = "My number is: " + num;
        }
    }

    void setGraph(IGraph@ graph)
    {
        @_graph = graph;
    }

    private bool _once;
    private IGraph@ _graph;
}

[node, desc:Tutorial node, name:Start]
class TutorialNodeOut : CNode 
{
    [type: out]
    prop<int> Out;

    void OnCreate()
    {
        print(name + " node created.");
        color(0.0, 0.3, 0.8); //set the color to red
        Out = 11;
    }

    void OnDestroy()
    {
        print(name + " node destroyed.");
    }

    void Update()
    {
        name = "Hello World!";
    }
}

[node, desc:Tutorial node, name:Bridge]
class TutorialNode : CNode 
{
    [type: in]
    prop<int> In;

    [type: out]
    prop<int> Out;

    void OnCreate() 
    { 
        print(name + " node created.");
        color(1.0, 0.0, 0.0); //set the color to red
    }

    void OnDestroy() 
    { 
        print(name + " node destroyed.");
    }

    void Update()
    {
        Out = In;
    }
}
