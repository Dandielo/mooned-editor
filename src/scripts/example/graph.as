#include "nodes.as"

[graph, name: Example Graph]
class BasicGraph : CGraph
{
    void OnCreate()
    {
        print(title + " graph created.");

        INode@ n = createNode("ExampleNode");
        n.move(4900, 4900); // move to center
    }

    void OnDestroy()
    {
        print(title + " graph destroyed.");
    }
} 
