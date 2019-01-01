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
    if (role == Qt::ItemDataRole::DisplayRole)
    {
        return _graph_element->displayText();
    }
    return { };
}

} // namespace editor
