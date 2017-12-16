#include "QScriptedGraphNode.h"
#include "project/scripted/QScriptedElementGraph.h"

editor::QScriptedGraphNode::QScriptedGraphNode(QScriptedElementGraph* graph, QProjectTreeNode* parent /*= nullptr*/)
    : QProjectTreeNode{ parent }
    , _graph{ graph }
{
}

editor::QScriptedGraphNode::~QScriptedGraphNode()
{
}

QString editor::QScriptedGraphNode::toString() const
{
    return _graph->displayText();
}
