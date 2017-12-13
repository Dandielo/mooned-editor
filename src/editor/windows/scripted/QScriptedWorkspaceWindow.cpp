#include "QScriptedWorkspaceWindow.h"

#include "graph/basic/QBasicGraphSerializer.h"
#include "graph/scripted/QScriptedGraph.h"

#include <cassert>
#include <QBuffer>
#include <QApplication>
#include <QClipboard>

asIScriptObject* QEditorWindowNewWorkspaceProxy(QScriptedWorkspaceWindow* window, std::string type_name)
{
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
}

QScriptedWorkspaceWindow::~QScriptedWorkspaceWindow()
{
}

void QScriptedWorkspaceWindow::initialize(Scripts::CScriptManager* script_manager)
{
    _script_manager = script_manager;

    auto* graph = _script_manager->CreateObject<editor::QScriptedGraph, 1>("BasicGraph");
    graph->initialize(_script_manager);

    _workspace = graph;
    _default_serializer = new editor::QBasicGraphSerializer();
    addDockWidget(Qt::LeftDockWidgetArea, _workspace);

    CallScriptMethod("OnCreate");
}

void QScriptedWorkspaceWindow::shutdown()
{
    CallScriptMethod("OnDestroy");

    auto* graph = dynamic_cast<editor::QScriptedGraph*>(_workspace);
    if (graph)
    {
        graph->shutdown();
    }

    delete _workspace;
    delete _default_serializer;
}

void QScriptedWorkspaceWindow::addWorkspace(QWorkspace* workspace)
{
}

QWorkspace* QScriptedWorkspaceWindow::activeWorkspace()
{
    return _workspace;
}

void QScriptedWorkspaceWindow::onSave()
{
    QBuffer buffer{};
    buffer.open(QBuffer::Text | QBuffer::WriteOnly);

    auto* graph = dynamic_cast<editor::QScriptedGraph*>(_workspace);
    if (graph != nullptr)
    {
        _default_serializer->serialize(&buffer, graph);
        QApplication::clipboard()->setText(buffer.buffer());
    }
}

void QScriptedWorkspaceWindow::onLoad()
{
    QBuffer buffer{};
    buffer.open(QBuffer::Text | QBuffer::WriteOnly);

    auto* graph = dynamic_cast<editor::QScriptedGraph*>(_workspace);
    if (graph != nullptr)
    {
        _default_serializer->serialize(&buffer, graph);
    }
}
