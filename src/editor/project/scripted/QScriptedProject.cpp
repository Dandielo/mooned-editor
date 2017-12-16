#include "QScriptedProject.h"
#include "QScriptedElementGraph.h"

#include "windows/QEditorMainWindow.h"
#include "windows/QWorkspaceWindow.h"

#include "project/models/QProjectModel.h"
#include "project/scripted/nodes/QScriptedProjectTree.h"
#include "project/scripted/nodes/QScriptedGraphNode.h"

#include <cassert>

static void destroyProjectElement(editor::QProjectElement* element)
{
    auto* graph_element = dynamic_cast<editor::QScriptedElementGraph*>(element);
    if (nullptr != graph_element)
    {
        graph_element->shutdown();
    }
    delete graph_element;
}

editor::QScriptedProject::QScriptedProject()
    : QProject{}
    , _script_manager{ nullptr }
    , _project_tree{ nullptr }
    , _model{ nullptr }
{
}

editor::QScriptedProject::QScriptedProject(QString name, QDir location)
    : QProject{ name, location }
    , _script_manager{ nullptr }
    , _project_tree{ nullptr }
    , _model{ nullptr }
{
}

editor::QScriptedProject::~QScriptedProject()
{
}

void editor::QScriptedProject::initialize(Scripts::CScriptManager* script_manager)
{
    _script_manager = script_manager;
}

void editor::QScriptedProject::initialize(QEditorMainWindow* mw)
{
    // Create the project tree and register it in the view model
    _model = mw->projectModel();
    _project_tree = new editor::QScriptedProjectTree{ this };

    _model->addProject(_project_tree);

    // Connect project signals
    connect(this, (void(editor::QScriptedProject::*)(QWorkspace*)) &editor::QScriptedProject::graphOpened, mw->workspaceWindow(), &QWorkspaceWindow::addWorkspace);
    connect(this, &QScriptedProject::projectTreeChanged, _model, &QProjectModel::projectTreeChanged);
}

void editor::QScriptedProject::shutdown()
{
    // Disconnect from all slots
    disconnect(nullptr, nullptr, nullptr, nullptr);

    // Delete the project tree
    _model->removeProject(_project_tree);
    delete _project_tree;

    // Delete all project elements
    for (auto* element : _elements)
    {
        destroyProjectElement(element);
    }
    _elements.clear();

    // Shutdown completed?
    _script_manager = nullptr;
}

void editor::QScriptedProject::newGraph(QString classname, QString name)
{
    auto* element = new editor::QScriptedElementGraph{ this, classname, name };

    if (!hasElement(element))
    {
        addElement(element);
        element->initialize(_script_manager);

        // Add the node to the tree
        _project_tree->add(new QScriptedGraphNode{ element, _project_tree });

        // Emit signals
        emit projectTreeChanged(_project_tree);
        emit graphOpened(element->graph());
    }
    else
    {
        delete element;
    }
}

void editor::QScriptedProject::onSave(QJsonObject& root)
{
}

void editor::QScriptedProject::onLoad(const QJsonObject& root)
{
}