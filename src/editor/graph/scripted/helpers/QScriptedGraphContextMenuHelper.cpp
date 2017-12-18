#include "QScriptedGraphContextMenuHelper.h"

#include "graph/scripted/dialogs/QCreateScriptedNodeDialog.h"
#include "graph/scripted/QScriptedGraph.h"
#include "graph/scripted/QScriptedNode.h"
#include "graph/scripted/QScriptedNodePin.h"

#include "graph/basic/QBasicNodeConnection.h"

#include <QGraphicsItem>
#include <QMenu>

#include <scriptarray.h>

editor::QScriptedGraphContextMenuHelper::QScriptedGraphContextMenuHelper()
    : QObject{ nullptr }
    , _graph{ nullptr }
    , _node_types{ }
{

}

editor::QScriptedGraphContextMenuHelper::~QScriptedGraphContextMenuHelper()
{

}

void editor::QScriptedGraphContextMenuHelper::initialize(Scripts::CScriptManager* script_manager, QScriptedGraph* graph)
{
    _graph = graph;

    // Get node queries
    auto* script_object = graph->ScriptObject();
    auto* engine = script_object->GetEngine();
    auto* ctx = engine->RequestContext();

    ctx->Prepare(graph->GetScriptMethod("NodeQueries", false));
    ctx->SetObject(script_object);
    ctx->Execute();

    std::vector<asITypeInfo*> node_types;
    auto* queries = reinterpret_cast<CScriptArray*>(ctx->GetReturnObject());
    if (nullptr != queries)
    {
        auto count = queries->GetSize();
        for (uint i = 0; i < count; ++i)
        {
            auto* str = reinterpret_cast<std::string*>(queries->At(i));
            auto queried_types = script_manager->QueryTypes(*str);

            for (auto* type : queried_types)
            {
                if (!_node_types.contains(type))
                {
                    _node_types.append(type);
                }
            }
        }
    }

    engine->ReturnContext(ctx);
}

void editor::QScriptedGraphContextMenuHelper::shutdown()
{
    _graph = nullptr;
}

void editor::QScriptedGraphContextMenuHelper::initializeContextMenu(QMenu* menu, QPointF pos, QGraphicsItem* item)
{
    if (nullptr == item)
    {
        initializeContextMenuNone(menu, pos);
    }
    else
    {
        auto* node_pin = dynamic_cast<editor::QScriptedNodePin*>(item);
        if (nullptr != node_pin)
        {
            initializeContextMenuNodePin(menu, node_pin);
        }

        QGraphicsItem* child = item;
        QScriptedNode* node = nullptr;
        while (nullptr != child && nullptr == node)
        {
            node = dynamic_cast<editor::QScriptedNode*>(child);
            child = child->parentItem();
        }

        if (nullptr != node)
        {
            initializeContextMenuNode(menu, node);
        }

        // We need only to check the node (atm)
        auto* node_conn = dynamic_cast<editor::QBasicNodeConnection*>(item);
        if (nullptr == node_conn)
        {
            initializeContextMenuNodeConnection(menu, node_conn);
        }
    }
}

void editor::QScriptedGraphContextMenuHelper::initializeContextMenuNone(QMenu* menu, QPointF pos)
{
    QAction* action;
    action = menu->addAction("New node");
    connect(action, &QAction::triggered, [&, pos]() {
        auto* dialog = new editor::QCreateScriptedNodeDialog{ _graph, _node_types };

        connect(dialog, &editor::QCreateScriptedNodeDialog::finished, [&, pos](QString classname) {
            auto* node = _graph->newScriptNode(classname);
            if (nullptr != node)
            {
                node->setPos(pos);
            }
        });

        dialog->show();
    });
}

void editor::QScriptedGraphContextMenuHelper::initializeContextMenuNode(QMenu* menu, QScriptedNode* node)
{
    QAction* action;
    action = menu->addAction("Delete");
    connect(action, &QAction::triggered, [&, node]() {
        _graph->removeNode(node);
    });
    action = menu->addAction("Disconnect all");
    connect(action, &QAction::triggered, [&, node]() {
        for (QNodePin* pin : node->inputPins())
        {
            pin->disconnectAll();
        }
        for (QNodePin* pin : node->outputPins())
        {
            pin->disconnectAll();
        }
    });
}

void editor::QScriptedGraphContextMenuHelper::initializeContextMenuNodePin(QMenu* menu, QScriptedNodePin* pin)
{

}

void editor::QScriptedGraphContextMenuHelper::initializeContextMenuNodeConnection(QMenu* menu, QBasicNodeConnection* conn)
{

}

