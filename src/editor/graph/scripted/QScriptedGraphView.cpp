#include "QScriptedGraphView.h"
#include "QScriptedGraph.h"

#include <QMenu>

#include <scriptstdstring.h>
#include <scriptarray.h>

editor::QScriptedGraphView::QScriptedGraphView(QWidget* parent /*= nullptr*/)
    : QBasicGraphView{ parent }
    , _script_manager{ nullptr }
    , _graph{ nullptr }
    , _node_types{ }
{
}

void editor::QScriptedGraphView::initialzie(Scripts::CScriptManager* script_manager, QScriptedGraph* graph)
{
    _script_manager = script_manager;
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
            auto queried_types = _script_manager->QueryTypes(*str);

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

void editor::QScriptedGraphView::shutdown()
{
    _graph = nullptr;
    _script_manager = nullptr;
}

void editor::QScriptedGraphView::contextMenuEvent(QContextMenuEvent* ev)
{
    QMenu menu;
    menu.addAction("New node");
    menu.exec(mapToGlobal(ev->pos()));
}

