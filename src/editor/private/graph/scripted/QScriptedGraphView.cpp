#include "QScriptedGraphView.h"
#include "QScriptedGraph.h"

#include <QMenu>

#include <scriptstdstring.h>
#include <scriptarray.h>

editor::QScriptedGraphView::QScriptedGraphView(QWidget* parent /*= nullptr*/)
    : QBasicGraphView{ parent }
    , _menu_helper{ }
    , _script_manager{ nullptr }
    , _graph{ nullptr }
{
}

void editor::QScriptedGraphView::initialzie(Scripts::CScriptManager* script_manager, QScriptedGraph* graph)
{
    _script_manager = script_manager;
    _graph = graph;

    // Initialize the context menu helper
    _menu_helper.initialize(_script_manager, _graph);
}

void editor::QScriptedGraphView::shutdown()
{
    _menu_helper.shutdown();

    _graph = nullptr;
    _script_manager = nullptr;
}

void editor::QScriptedGraphView::contextMenuEvent(QContextMenuEvent* ev)
{
    QGraphicsItem* item = itemAt(ev->pos());
    QPointF scene_pos = mapToScene(ev->pos());

    QMenu menu;
    _menu_helper.initializeContextMenu(&menu, scene_pos, item);
    menu.exec(mapToGlobal(ev->pos()));
}

