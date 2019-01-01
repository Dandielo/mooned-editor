

class TutorialNode : CNode
{
    void OnCreate()
    {
        color(1.0, 0.0, 0.0);
    }
}

[node, desc:node.tutorial, name:Start Node]
class TutorialNodeStart : TutorialNode
{
    [type: out]
    prop<int> Out;
}

[node, desc:node.tutorial, name:Calc Node]
class TutorialNodeCalculate : TutorialNode
{
    [type: in]
    prop<int> One;

    [type: in]
    prop<int> Two;

    [type: out, noedit]
    prop<int> Out;

    void Update()
    {
        int one = One;
        int two = Two;

        Out = one + two;
    }
}

[node, desc:node.tutorial, name:End Node]
class TutorialNodeEnd : TutorialNode
{
    [type: in]
    prop<int> In;
}

[node, desc:math, name:Add]
class IntAddNode : CNode
{
    [type: in]
    prop<int> _n1;

    [type: in]
    prop<int> _n2;

    [type: out, noedit]
    prop<int> _sum;

    void Update()
    {
        int one = _n1;
        int two = _n2;

        _sum = one + two;
    }
}
