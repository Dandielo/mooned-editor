#pragma once
#include "project/models/QProjectTreeNode.h"

namespace editor
{
    class QScriptedElementGraph;
    class QScriptedGraphNode : public editor::QProjectTreeNode
    {
    public:
        QScriptedGraphNode(QScriptedElementGraph* graph, QProjectTreeNode* parent);
        virtual ~QScriptedGraphNode();

        virtual QString toString() const override;

    protected:
        QScriptedElementGraph* _graph;
    };
}
