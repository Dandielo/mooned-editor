#pragma once
#include <project/models/QProjectTreeNode.h>

namespace editor
{

//! The project element class.
class QScriptedElementGraph;

//! The tree node class representing a scripted graph project element.
class ScriptedGraphNode : public editor::ProjectTreeNode
{
public:
    ScriptedGraphNode(QScriptedElementGraph* graph, ProjectTreeNode* parent);

    //! \returns A value for the given item data role.
    auto value(Qt::ItemDataRole role) const noexcept -> QVariant override;

protected:
    QScriptedElementGraph* _graph_element;
};

} // namespace editor
