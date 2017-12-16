#include "nodes.as"

[graph, name: Example Graph]
class BasicGraph : CGraph
{
    void OnCreate()
    {
        // print(title + " graph created.");

        // INode@ n = createNode("TutorialNode");
        // n.move(4900, 4900); // move to center

        // @n = createNode("TutorialNodeIn");
        // n.move(5100, 4900); // move to center

        // TutorialNodeIn@ tn = cast<TutorialNodeIn@>(n);
        // if (tn !is null)
        // {
        //     tn.setGraph(this);
        // }
    }

    void OnDestroy()
    {
        print(title + " graph destroyed.");
    }
} 
