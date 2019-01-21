#include "QScriptedGraph.h"
#include "QScriptedNode.h"

using editor::QScriptedGraph;

namespace editor
{
namespace detail
{

QScriptedGraph* scripted_graph_factory()
{
    auto ctx = asGetActiveContext();

    // Get the factory data object
    auto udata = reinterpret_cast<const QScriptedGraph::FactoryData*>(ctx->GetUserData(0x01));
    assert(udata != nullptr);

    auto func = ctx->GetFunction(0);
    if (func->GetObjectType() == 0 || strcmp(func->GetObjectType()->GetName(), "CGraph") != 0)
    {
        std::string msg = "Invalid attempt to manually instantiate ";
        msg += "CProjectNative";
        ctx->SetException(msg.c_str());
        return nullptr;
    }

    auto obj = reinterpret_cast<asIScriptObject*>(ctx->GetThisPointer(0));
    return new QScriptedGraph{ editor::script::ScriptObject::from_native(obj) };
}

} // namespace detail
} // namespace editor

asIScriptObject* asGraphCreateNodeProxy(QScriptedGraph* wks, std::string nodeclass)
{
    auto* node = wks->newScriptNode(QString::fromStdString(nodeclass))->script_object().native();
    node->AddRef();
    return node;
}

void asGraphSetTitleProxy(QScriptedGraph* wks, std::string title)
{
    wks->setWindowTitle(QString::fromStdString(title));
}

std::string asGraphGetTitleProxy(QScriptedGraph* wks)
{
    return wks->windowTitle().toLocal8Bit().data();
}

std::string asGraphGetNameProxy(QScriptedGraph* wks)
{
    return wks->name().toLocal8Bit().data();
}

void editor::QScriptedGraph::registerTypeInterface(asIScriptEngine* engine)
{
    engine->RegisterObjectBehaviour(ClassName, asBEHAVE_FACTORY, "CGraphNative@ f()", asFUNCTION(detail::scripted_graph_factory), asCALL_CDECL);

    engine->RegisterObjectMethod(ClassName, "INode@ createNode(string)", asFUNCTION(asGraphCreateNodeProxy), asCALL_CDECL_OBJFIRST);

    // Properties
    engine->RegisterObjectMethod(ClassName, "string get_name()", asFUNCTION(asGraphGetNameProxy), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod(ClassName, "string get_title()", asFUNCTION(asGraphGetTitleProxy), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod(ClassName, "void set_title(string)", asFUNCTION(asGraphSetTitleProxy), asCALL_CDECL_OBJFIRST);

    // API
    engine->RegisterInterface("IGraph");
    engine->RegisterInterfaceMethod("IGraph", "array<string> NodeQueries()");

    engine->RegisterInterfaceMethod("IGraph", "string get_name()");
    engine->RegisterInterfaceMethod("IGraph", "string get_title()");
    engine->RegisterInterfaceMethod("IGraph", "void set_title(string)");

    engine->RegisterInterfaceMethod("IGraph", "INode@ createNode(string)");

    // Handles
    engine->RegisterInterfaceMethod("IGraph", "void OnCreate()");
    engine->RegisterInterfaceMethod("IGraph", "void OnDestroy()");
    engine->RegisterInterfaceMethod("IGraph", "void OnNodeAdded(INode@)");
    engine->RegisterInterfaceMethod("IGraph", "void OnNodeRemoved(INode@)");
}
