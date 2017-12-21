#include "QScriptedProject.h"

#include "windows/QEditorMainWindow.h"
#include "windows/QWorkspaceWindow.h"

#include "project/models/QProjectModel.h"
#include "project/dialogs/QDialogExportProjectElement.h"
#include "project/scripted/nodes/QScriptedProjectTree.h"
#include "project/scripted/nodes/QScriptedGraphNode.h"
#include "project/scripted/elements/QScriptedElementGraph.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
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

static void initializeProjectElement(editor::QProjectElement* element, Scripts::CScriptManager* script_manager, editor::QProjectTree* project_tree)
{
    auto* graph_element = dynamic_cast<editor::QScriptedElementGraph*>(element);
    if (nullptr != graph_element)
    {
        graph_element->initialize(script_manager);
        graph_element->load();

        // Add the node to the tree
        project_tree->add(new editor::QScriptedGraphNode{ graph_element, project_tree });
    }
}

static void serializeProjectElement(QJsonObject& graph_object, editor::QProjectElement* element)
{
    auto* graph = dynamic_cast<editor::QScriptedElementGraph*>(element);
    if (nullptr != graph)
    {
        graph_object.insert("name", graph->graphName());
        graph_object.insert("class", graph->className());
    }
    else
    {
        graph_object.insert("name", element->name());
    }
}

static editor::QProjectElement* findProjectElementByDisplayText(QMap<QString, editor::QProjectElement*>& elements, QString text)
{
    editor::QProjectElement* result = nullptr;
    for (editor::QProjectElement* element : elements)
    {
        if (element->displayText() == text)
        {
            result = element;
            break;
        }
    }
    return result;
}


editor::QScriptedProject::QScriptedProject(asIScriptObject* object)
    : QProject{ }
    , CNativeScriptObject{ object }
    , _script_manager{ nullptr }
    , _project_tree{ nullptr }
    , _model{ nullptr }
{
}

editor::QScriptedProject::~QScriptedProject()
{
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

    // Initialize all loaded elements
    for (auto* element : _elements)
    {
        initializeProjectElement(element, _script_manager, _project_tree);
    }

    emit projectTreeChanged(_project_tree);
}

void editor::QScriptedProject::initialize(Scripts::CScriptManager* script_manager)
{
    _script_manager = script_manager;
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

void editor::QScriptedProject::openElement(QString name)
{
    auto* element = findProjectElementByDisplayText(_elements, name);
    if (nullptr != element)
    {
        auto* graph_element = dynamic_cast<QScriptedElementGraph*>(element);
        if (nullptr != graph_element)
        {
            emit graphOpened(graph_element->graph());
        }
    }
}

void editor::QScriptedProject::saveElement(QString name)
{
    auto* element = findProjectElementByDisplayText(_elements, name);
    if (nullptr != element)
    {
        element->save();
    }
}

void editor::QScriptedProject::deleteElement(QString name)
{
    auto* element = findProjectElementByDisplayText(_elements, name);
    if (nullptr != element)
    {

    }
}

void editor::QScriptedProject::exportElement(QString name)
{
    auto* element = findProjectElementByDisplayText(_elements, name);
    if (nullptr != element)
    {
        auto* graph_element = dynamic_cast<editor::QScriptedElementGraph*>(element);

        auto* dialog = new editor::QDialogExportProjectElement{ element };
        dialog->show();
    }
}

void editor::QScriptedProject::onSave(QJsonObject& root)
{
    QJsonArray graphs_array;

    for (auto* element : _elements)
    {
        element->save();

        QJsonObject graph_object;
        serializeProjectElement(graph_object, element);
        graphs_array.append(graph_object);
    }

    root.insert("graphs", graphs_array);
}

void editor::QScriptedProject::onLoad(const QJsonObject& root)
{
    if (!root.contains("graphs"))
    {
        return;
    }

    QJsonArray graphs_array = root.value("graphs").toArray();

    uint count = graphs_array.count();
    for (uint i = 0; i < count; ++i)
    {
        QJsonValue graph_value = graphs_array.at(i);
        assert(graph_value.isObject());
        QJsonObject graph_object = graph_value.toObject();

        QString graph_name = graph_object.value("name").toString();
        QString class_name = graph_object.value("class").toString();

        if (!graph_name.isEmpty() && !class_name.isEmpty())
        {
            addGraph(class_name, graph_name);
        }
    }
}

void editor::QScriptedProject::addGraph(QString classname, QString name)
{
    auto* element = new editor::QScriptedElementGraph{ this, classname, name };
    assert(!hasElement(element));

    addElement(element);
}
