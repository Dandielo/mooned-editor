
[node, desc:A basic node, name:Example]
class ExampleNode : CNode 
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
}
