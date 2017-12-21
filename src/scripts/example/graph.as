#include "nodes.as"


[graph, name: Example Graph]
class ExampleGraph : CGraph
{
    array<string> NodeQueries() 
    { 
        return array<string> = { 
            "[node] : TutorialNode",
            "[node] : CNode"
        }; 
    }
    
    void OnCreate()
    {
        print("Example graph created.");
    }

    void OnDestroy()
    {
        print("Example graph destroyed.");
    }
} 
