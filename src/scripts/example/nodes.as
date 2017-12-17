
[node, desc:node.tutorial, name:Start Node]
class TutorialNodeStart : CNode 
{
    void OnCreate()
    {
        color(0.0, 0.8, 0.3); 
    }
}

[node, desc:node.tutorial, name:Calc Node]
class TutorialNodeCalculate : CNode 
{
    void OnCreate()
    {
        color(0.0, 0.3, 0.8); 
    }
}

[node, desc:node.tutorial, name:End Node]
class TutorialNodeEnd : CNode 
{
    void OnCreate() 
    { 
        color(1.0, 0.0, 0.0); 
    }
}


class TutorialNode : CNode 
{
}

[node, desc:node.tutorial, name:My Tutorial Node]
class MyTutorialNode : TutorialNode 
{
    void OnCreate() 
    { 
        color(1.0, 0.0, 0.0); 
    }
}
