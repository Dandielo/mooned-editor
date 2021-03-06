#include "QScriptedWorkspaceWindow.h"

#include "graph/basic/QBasicGraphSerializer.h"
#include "graph/scripted/QScriptedGraph.h"

#include <cassert>
#include <QBuffer>
#include <QApplication>
#include <QClipboard>

static void releaseWorkspace(QWorkspace* workspace)
{
    //auto* graph = dynamic_cast<editor::QScriptedGraph*>(workspace);
    //if (graph != nullptr)
    //{
    //    graph->shutdown();
    //}

    //delete workspace;
}

static QWorkspace* findWorkspaceByName(QVector<QWorkspace*>& workspaces, QString name)
{
    QWorkspace* result = nullptr;
    for (QWorkspace* wks : workspaces)
    {
        if (wks->name() == name)
        {
            result = wks;
            break;
        }
    }
    return result;
}

asIScriptObject* QEditorWindowNewWorkspaceProxy(QScriptedWorkspaceWindow* window, std::string type_name)
{
    return nullptr;
}

auto scripted_workspace_window_factory() noexcept -> QScriptedWorkspaceWindow*
{
    auto ctx = asGetActiveContext();

    // Get the factory data object
    auto udata = reinterpret_cast<const QScriptedWorkspaceWindow::FactoryData*>(ctx->GetUserData(0x01));
    assert(udata != nullptr);

    auto func = ctx->GetFunction(0);
    if (func->GetObjectType() == 0 || strcmp(func->GetObjectType()->GetName(), "CWorkspaceWindow") != 0)
    {
        std::string msg = "Invalid attempt to manually instantiate ";
        msg += "CProjectNative";
        ctx->SetException(msg.c_str());
        return nullptr;
    }

    auto obj = reinterpret_cast<asIScriptObject*>(ctx->GetThisPointer(0));
    return new QScriptedWorkspaceWindow{ editor::script::ScriptObject::from_native(obj)  };
}

void QScriptedWorkspaceWindow::registerTypeInterface(asIScriptEngine* engine)
{
    engine->RegisterInterface("IWorkspaceWindow");
    engine->RegisterInterfaceMethod("IWorkspaceWindow", "void OnCreate()");
    engine->RegisterInterfaceMethod("IWorkspaceWindow", "void OnDestroy()");
    //engine->RegisterInterfaceMethod("IWorkspace", "void OnNewWorkspace(IWorkspace@)");

    assert(strcmp(ClassName, "CWorkspaceWindowNative") == 0);
    engine->RegisterObjectBehaviour("CWorkspaceWindowNative", asBEHAVE_FACTORY, "CWorkspaceWindowNative@ f()", asFUNCTION(scripted_workspace_window_factory), asCALL_CDECL);
    //engine->RegisterObjectMethod("CWorkspaceWindowNative", "IWorkspaceWindow@ newWorkspaceWindow(string)", asFUNCTION(QEditorWindowNewWorkspaceProxy), asCALL_CDECL_OBJFIRST);
}

QScriptedWorkspaceWindow::QScriptedWorkspaceWindow(editor::script::ScriptObject&& obj)
    : QWorkspaceWindow{ }
    , CNativeScriptObject{ std::move(obj) }
{ }

QScriptedWorkspaceWindow::~QScriptedWorkspaceWindow()
{
}

void QScriptedWorkspaceWindow::initialize(Scripts::CScriptManager* script_manager)
{
    _script_manager = script_manager;
    auto types = _script_manager->query_types("[graph] : CGraph");

    for (const auto& type : types)
    {
        _workspace_types.append(type.name().c_str());
    }

    CallScriptMethod("OnCreate");
}

void QScriptedWorkspaceWindow::shutdown()
{
    CallScriptMethod("OnDestroy");

    for (auto* workspace : _workspaces)
    {
        releaseWorkspace(workspace);//#todo does nothing atm
    }
}

void QScriptedWorkspaceWindow::addWorkspace(QWorkspace* workspace)
{
    assert(nullptr != workspace);

    if (!_workspaces.contains(workspace))
    {
        _workspaces.append(_workspaces);

        addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, workspace);
        setActiveWorkspace(workspace);
    }
}

void QScriptedWorkspaceWindow::closeWorkspace(QString name)
{
    auto* workspace = findWorkspaceByName(_workspaces, name);
    if (workspace)
    {
        _workspaces.removeAt(_workspaces.indexOf(workspace));

        if (_active_workspace == workspace && !_workspaces.isEmpty())
        {
            setActiveWorkspace(_workspaces.last());
        }
        else if(_active_workspace == workspace)
        {
            setActiveWorkspace(nullptr);
        }

        releaseWorkspace(workspace); // #todo does nothing atm
    }
}

void QScriptedWorkspaceWindow::setActiveWorkspace(QWorkspace* workspace)
{
    _active_workspace = workspace;
    if (nullptr != _active_workspace)
    {
        _active_workspace->setFocus();
        _active_workspace->show();
    }
}

void QScriptedWorkspaceWindow::createWorkspace()
{
}

//void QScriptedWorkspaceWindow::onSave()
//{
//    QBuffer buffer{};
//    buffer.open(QBuffer::Text | QBuffer::WriteOnly);
//
//    auto* graph = dynamic_cast<editor::QScriptedGraph*>(_workspace);
//    if (graph != nullptr)
//    {
//        _default_serializer->serialize(&buffer, graph);
//        QApplication::clipboard()->setText(buffer.buffer());
//    }
//}
//
//void QScriptedWorkspaceWindow::onLoad()
//{
//    QBuffer buffer{};
//    buffer.open(QBuffer::Text | QBuffer::WriteOnly);
//
//    auto* graph = dynamic_cast<editor::QScriptedGraph*>(_workspace);
//    if (graph != nullptr)
//    {
//        _default_serializer->serialize(&buffer, graph);
//    }
//}
