#include "QScriptedProject.h"

#include "windows/QEditorMainWindow.h"
#include "windows/QWorkspaceWindow.h"

#include "project/models/QProjectModel.h"
#include "project/dialogs/QDialogExportProjectElement.h"
#include "project/scripted/nodes/QScriptedProjectTree.h"
#include "project/scripted/nodes/QScriptedGraphNode.h"
#include "project/scripted/elements/QScriptedElementGraph.h"

#include <QPointer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

#include <scriptarray.h>
#include <cassert>

namespace editor
{
//
//static void destroyProjectElement(QProject::QProjectElementPtr element)
//{
//    auto* scripted_element = dynamic_cast<QScriptedElementGraph*>(element.data());
//    if (scripted_element)
//    {
//        scripted_element->shutdown();
//        scripted_element->deleteLater(); // #todo needed?
//    }
//}

static void loadProjectElement(editor::QProjectElement* element, Scripts::CScriptManager* script_manager, editor::ProjectTreeRoot* project_tree)
{
    auto* graph_element = dynamic_cast<editor::QScriptedElementGraph*>(element);
    if (nullptr != graph_element)
    {
        graph_element->initialize(script_manager);
        graph_element->load();

        // Add the node to the tree
        project_tree->add(std::make_unique<editor::ScriptedGraphNode>(graph_element, project_tree));
    }
}

static void saveProjectElement(QJsonObject& graph_object, editor::QProjectElement* element)
{
    QProject* project = element->project();
    auto* graph = dynamic_cast<editor::QScriptedElementGraph*>(element);
    if (nullptr != graph)
    {
        graph_object.insert("file", project->location().relativeFilePath(element->fileinfo().absoluteFilePath()));
        graph_object.insert("class", graph->class_name());
        graph->save();
    }
    else
    {
        graph_object.insert("name", element->name());
    }
}

auto findProjectElementByDisplayText(QMap<QString, QProject::QProjectElementPtr>& elements, QString text) -> QProject::QProjectElementPtr
{
    QProject::QProjectElementPtr result;
    for (const auto& element : elements)
    {
        if (element->value(Qt::ItemDataRole::DisplayRole).toString() == text)
        {
            result = element;
            break;
        }
    }
    return result;
}

//////////////////////////////////////////////////////////////////////////

QScriptedProject::QScriptedProject(editor::script::ScriptObject&& object, QFileInfo project_file, QString script_class)
    : QProject{ std::move(project_file) }
    , CNativeScriptObject{ std::move(object) }
    , _script_class{ std::move(script_class) }
{ }

auto editor::QScriptedProject::class_name() const noexcept -> QString
{
    return _script_class;
}

auto editor::QScriptedProject::value(Qt::ItemDataRole role) const noexcept -> QVariant
{
    if (role == Qt::ItemDataRole::DisplayRole)
    {
        return settings().get("project.display_name");
    }
    return { };
}

} // namespace editor

void editor::QScriptedProject::initialize(QEditorMainWindow* mw) noexcept
{
    // Initialize internal data
    gatherExporters();

    // Create the project tree and register it in the view model
    _model = mw->projectModel();
    _project_tree = std::make_unique<editor::ScriptedProjectTreeRoot>(this);

    _model->add_project(_project_tree);

    // Connect project signals
    connect(this, (void(editor::QScriptedProject::*)(QWorkspace*)) &editor::QScriptedProject::graphOpened, mw->workspaceWindow(), &QWorkspaceWindow::addWorkspace);
    connect(this, &QScriptedProject::projectTreeChanged, _model, &QProjectModel::projectTreeChanged);

    // Initialize all loaded elements
    for (auto& element : elements())
    {
        loadProjectElement(element, _script_manager, _project_tree.get());
    }

    emit projectTreeChanged(_project_tree.get());
}

void editor::QScriptedProject::setScriptManager(Scripts::CScriptManager* script_manager)
{
    _script_manager = script_manager;
}

void editor::QScriptedProject::shutdown()
{
    // Disconnect from all slots
    disconnect(nullptr, nullptr, nullptr, nullptr);

    // Delete the project tree
    _model->remove_project(_project_tree);
    _project_tree.reset();

    // Shutdown completed?
    _script_manager = nullptr;
}

void editor::QScriptedProject::newGraph(QString classname, QString name)
{
    if (!contains_element(name))
    {
        QScriptedElementGraph* element = new editor::QScriptedElementGraph{ this, classname, name };
        element->initialize(_script_manager);

        // Add the node to the tree
        _project_tree->add(std::make_unique<editor::ScriptedGraphNode>(element, _project_tree.get()));

        // Add the element to the project.
        add_element(element);

        // Emit signals
        emit projectTreeChanged(_project_tree.get());
        emit graphOpened(element->graph());
    }
}

void editor::QScriptedProject::openElement(QString name)
{
    if (auto element = findProjectElementByDisplayText(elements(), name))
    {
        auto* graph_element = dynamic_cast<QScriptedElementGraph*>(element.data());
        if (nullptr != graph_element)
        {
            emit graphOpened(graph_element->graph());
        }
    }
}

void editor::QScriptedProject::saveElement(QString name)
{
    if (auto element = findProjectElementByDisplayText(elements(), name))
    {
        element->save();
    }
}

void editor::QScriptedProject::deleteElement(QString name)
{
    remove_element(name);
}

void editor::QScriptedProject::exportElement(QString name)
{
    if (auto element = findProjectElementByDisplayText(elements(), name))
    {
        auto* graph_element = dynamic_cast<editor::QScriptedElementGraph*>(element.data());

        auto* dialog = new editor::QDialogExportProjectElement{ _script_manager, _exporters, settings().get("export.location").toString(), element };
        dialog->show();
    }
}

void editor::QScriptedProject::onSave(QJsonObject& root) const
{
    QJsonArray graphs_array;

    for (auto& element : elements())
    {
        element->save();

        QJsonObject graph_object;
        saveProjectElement(graph_object, element);
        graphs_array.append(graph_object);
    }

    root.insert("graphs", graphs_array);
}

void editor::QScriptedProject::onLoad(const QJsonObject& root, const QVersionNumber& loaded_ver)
{
    if (!root.contains("graphs"))
    {
        return;
    }

    // Helper function which will chose the right loader between different project versions.
    std::function<void(const QJsonObject&)> element_loader;

    if (loaded_ver >= version())
    {
        element_loader = [&](const QJsonObject& element)
        {
            QFileInfo graph_file = location().filePath(element.value("file").toString());
            QString graph_class = element.value("class").toString();

            add_element(new QScriptedElementGraph{ this, graph_class, graph_file });
        };
    }
    else // The old 'alpha' version loader.
    {
        element_loader = [&](const QJsonObject& element)
        {
            QString graph_name = element.value("name").toString();
            QString graph_class = element.value("class").toString();

            if (!graph_name.isEmpty() && !graph_class.isEmpty())
            {
                addGraph(graph_class, graph_name);
            }
        };
    }


    QJsonArray graphs_array = root.value("graphs").toArray();

    uint count = graphs_array.count();
    for (uint i = 0; i < count; ++i)
    {
        QJsonValue graph_value = graphs_array.at(i);
        assert(graph_value.isObject());

        // Run the loader
        element_loader(graph_value.toObject());
    }
}

void editor::QScriptedProject::addGraph(QString classname, QString name)
{
    auto* element = new editor::QScriptedElementGraph{ this, classname, name };
    assert(!contains_element(name));

    add_element(element);
}

namespace editor
{

void editor::QScriptedProject::gatherExporters() noexcept
{
    // Get node queries
    auto* script_engine = _script_manager->engine().native();
    auto* call_context = script_engine->RequestContext();

    // Prepare the function call
    call_context->Prepare(GetScriptMethod("AvailableExporters", false));
    call_context->SetObject(script_object().native());

    // Execute the function
    call_context->Execute();

    // Get the result object
    auto* const exporters_array = reinterpret_cast<CScriptArray*>(call_context->GetReturnObject());

    // Gather the results
    if (exporters_array != nullptr)
    {
        // Get the exporter interface type info object.
        auto* const exporter_interface_type = script_engine->GetTypeInfoByName("IExporter");
        assert(exporter_interface_type != nullptr); // If this interface does not exist, it migh been renamed?

        // #todo Make a better abstraction for this thing...
        const auto exporter_count = exporters_array->GetSize();
        for (uint32_t i = 0; i < exporter_count; ++i)
        {
            // Get the exporter type by name
            auto exporter_type_name = QString::fromStdString(*reinterpret_cast<const std::string*>(exporters_array->At(i)));
            auto& exporter_type = _script_manager->GetTypeInfo(exporter_type_name.toStdString());

            // Is the type defined and implements the proper interface?
            if (exporter_type && exporter_type->Implements(exporter_interface_type))
            {
                _exporters.append(exporter_type_name);
            }
            else
            {
                static const QString log_message { "The given exporter type name '%1' is not valid: %2!" };

                QString warning_details{ "type not found" };
                if (exporter_type)
                {
                    warning_details = "type does not extend the 'CExporter' type";
                }

                qWarning() << "[editor][warning]" << log_message.arg(exporter_type_name, warning_details);
            }
        }

        //for (uint i = 0; i < count; ++i)
        //{
        //    auto* str = reinterpret_cast<std::string*>(queries->At(i));
        //    auto queried_types = script_manager->QueryTypes(*str);

        //    for (auto* type : queried_types)
        //    {
        //        if (!_node_types.contains(type))
        //        {
        //            _node_types.append(type);
        //        }
        //    }
        //}
    }

    script_engine->ReturnContext(call_context);
}

} // namespace editor
