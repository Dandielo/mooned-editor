#pragma once
#include "graph/basic/QBasicGraphView.h"
#include "scripts/CScriptManager.h"

namespace editor
{
    class QScriptedGraph;
    class QScriptedGraphView : public QBasicGraphView
    {
        Q_OBJECT;

    public:
        QScriptedGraphView(QWidget* parent = nullptr);
        virtual ~QScriptedGraphView() override = default;

        void initialzie(Scripts::CScriptManager* script_manager, QScriptedGraph* graph);
        void shutdown();

    protected:
        virtual void contextMenuEvent(QContextMenuEvent* ev) override;

    private:
        Scripts::CScriptManager* _script_manager;
        QScriptedGraph* _graph;

        QVector<asITypeInfo*> _node_types;
    };
}
