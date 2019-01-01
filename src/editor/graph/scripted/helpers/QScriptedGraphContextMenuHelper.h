#pragma once
#include <QObject>
#include <QVector>

#include "scripts/CScriptManager.h"

class asITypeInfo;

class QMenu;
class QGraphicsItem;

namespace editor
{
    class QScriptedGraph;
    class QScriptedNode;
    class QScriptedNodePin;
    class QBasicNodeConnection;

    class QScriptedGraphContextMenuHelper : public QObject
    {
        Q_OBJECT;

    public:
        QScriptedGraphContextMenuHelper();
        ~QScriptedGraphContextMenuHelper();

        void initialize(Scripts::CScriptManager* script_manager, QScriptedGraph* graph);
        void shutdown();

        void initializeContextMenu(QMenu* menu, QPointF pos, QGraphicsItem* item);

    protected:
        void initializeContextMenuNone(QMenu* menu, QPointF pos);
        void initializeContextMenuNode(QMenu* menu, QScriptedNode* node);
        void initializeContextMenuNodePin(QMenu* menu, QScriptedNodePin* pin);
        void initializeContextMenuNodeConnection(QMenu* menu, QBasicNodeConnection* conn);

    private:
        QScriptedGraph* _graph;
        QVector<editor::script::Type> _node_types;
    };
}