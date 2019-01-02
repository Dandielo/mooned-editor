#include "QScriptedGraphNode.h"
#include "project/scripted/elements/QScriptedElementGraph.h"

namespace editor
{

ScriptedGraphNode::ScriptedGraphNode(QScriptedElementGraph* graph, ProjectTreeNode* parent)
    : ProjectTreeNode{ parent }
    , _graph_element{ graph }
{ }

auto ScriptedGraphNode::value(Qt::ItemDataRole role) const noexcept -> QVariant
{
    return _graph_element->value(role);
}

} // namespace editor
