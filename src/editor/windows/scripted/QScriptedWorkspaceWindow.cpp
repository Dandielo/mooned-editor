#include "QScriptedWorkspaceWindow.h"
#include "graph/scripted/QScriptedGraph.h"

#include <cassert>

asIScriptObject* QEditorWindowNewWorkspaceProxy(QScriptedWorkspaceWindow* window, std::string type_name)
{
    //auto wks = window->newScriptWorkspace(type_name)->ScriptObject();
    //wks->AddRef();
    return nullptr;
}

void QScriptedWorkspaceWindow::registerTypeInterface(asIScriptEngine* engine)
{
    engine->RegisterInterface("IWorkspaceWindow");
    engine->RegisterInterfaceMethod("IWorkspaceWindow", "void OnCreate()");
    engine->RegisterInterfaceMethod("IWorkspaceWindow", "void OnDestroy()");
    //engine->RegisterInterfaceMethod("IWorkspace", "void OnNewWorkspace(IWorkspace@)");

    assert(strcmp(ClassName, "CWorkspaceWindowNative") == 0);
    //engine->RegisterObjectMethod("CWorkspaceWindowNative", "IWorkspaceWindow@ newWorkspaceWindow(string)", asFUNCTION(QEditorWindowNewWorkspaceProxy), asCALL_CDECL_OBJFIRST);
}

QScriptedWorkspaceWindow::QScriptedWorkspaceWindow(asIScriptObject* obj) : QWorkspaceWindow(), CNativeScriptObject(obj)
{
    CallScriptMethod("OnCreate");
}

QScriptedWorkspaceWindow::~QScriptedWorkspaceWindow()
{
    CallScriptMethod("OnDestroy");
}

void QScriptedWorkspaceWindow::initialize(Scripts::CScriptManager* script_manager)
{
    _script_manager = script_manager;

    auto* graph = _script_manager->CreateObject<editor::QScriptedGraph, 1>("BasicGraph");
    graph->initialize(_script_manager);

    _workspace = graph;
    addDockWidget(Qt::LeftDockWidgetArea, _workspace);
}

void QScriptedWorkspaceWindow::addWorkspace(QWorkspace* workspace)
{
}

QWorkspace* QScriptedWorkspaceWindow::activeWorkspace()
{
    return _workspace;
}
