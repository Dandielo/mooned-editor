#pragma once
#include "graph/basic/QBasicGraphView.h"
#include "graph/scripted/helpers/QScriptedGraphContextMenuHelper.h"

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
        QScriptedGraphContextMenuHelper _menu_helper;

        Scripts::CScriptManager* _script_manager;
        QScriptedGraph* _graph;
    };
}
