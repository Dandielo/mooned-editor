#include "QEditorMainWindow.h"
#include "scripted/QScriptedWorkspaceWindow.h"
#include "project/scripted/QScriptedProject.h"
#include "project/dialogs/QDialogNewProject.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
#include <QStandardPaths>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <algorithm>

static editor::QProject* findProjectByName(QVector<editor::QProject*>& projects, QString name)
{
    editor::QProject* result = nullptr;
    for (editor::QProject* prj : projects)
    {
        if (prj->name() == name)
        {
            result = prj;
            break;
        }
    }
    return result;
}

static void setup_main_menu_bar(editor::QEditorMainWindow* editor, Ui::QTWindow* ui, QMenuBar* menu)
{
    QAction* action = ui->menuEdit->addAction("&Save");
    action->setShortcut(QKeySequence::Save);
    //QApplication::connect(action, &QAction::triggered, editor, &editor::QEditorMainWindow::onSave);

    action = ui->menuEdit->addAction("&Open");
    action->setShortcut(QKeySequence::Open);
    //QApplication::connect(action, &QAction::triggered, editor, &editor::QEditorMainWindow::onLoad);

    action = ui->menuEdit->addAction("&Export");
    //QApplication::connect(action, &QAction::triggered, editor, &editor::QEditorMainWindow::onLoad);

    // Project actions
    action = ui->menuFile->addAction("&New project");
    QApplication::connect(action, &QAction::triggered, editor, [editor]()
        {
            (new editor::QDialogNewProject{ editor })->show();
        });
    action = ui->menuFile->addAction("&Open project");
    QApplication::connect(action, &QAction::triggered, editor, &editor::QEditorMainWindow::onOpenProject);
    action = ui->menuFile->addAction("&Save project");
    QApplication::connect(action, &QAction::triggered, editor, (void(editor::QEditorMainWindow::*)())&editor::QEditorMainWindow::onSaveProject);
}

namespace editor
{
namespace detail
{

//! A custom deleter for QWorkspaceWindow unique_ptr types.
void custom_qworkspacewindow_deleter(QWorkspaceWindow* workspace_window) noexcept
{
    auto* window = dynamic_cast<QScriptedWorkspaceWindow*>(workspace_window);
    window->shutdown();
    window->deleteLater();
}

//! A custom deleter for QProject unique_ptr types.
void custom_qproject_deleter(QProject* project) noexcept
{
    project->save();
    project->deleteLater();
}

//! A factory function used for loading or creating projects based on script definitions.
auto scripted_project_factory(Scripts::CScriptManager* script_manager, const editor::project::FactoryData& data) noexcept -> QProject*
{
    auto factory_userdata = QScriptedProject::FactoryData{ data.file, QString::fromStdString(data.class_name) };

    // Create the project object
    auto* project = script_manager->create_object(data.class_name, factory_userdata);
    project->setScriptManager(script_manager);
    project->load();

    return project;
}

} // namespace detail

QEditorMainWindow::QEditorMainWindow() noexcept
    : QMainWindow{ }
    , _ui{ std::make_unique<Ui::QTWindow>() }
    , _script_manager{ std::make_unique<Scripts::CScriptManager>() }
    , _editor_settings{ QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) }
{
    // Setup default values for editor settings
    _editor_settings.set_default("projects.location", QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/meditor/projects"));


    // Create required directories
    QDir{ _editor_settings.get("projects.location").toString() }.mkpath(".");


    // Setup the UI
    _ui->setupUi(this);
    setup_main_menu_bar(this, _ui.get(), _ui->menuBar);

    // Load the main script file
    _script_manager->load("../../src/scripts/main.as");


    // Create the project model and assign it to the project tree view.
    _project_model = std::make_unique<QProjectModel>();
    _ui->projectsFileTree->setModel(_project_model.get());
    _ui->projectsFileTree->setContextMenuPolicy(Qt::NoContextMenu); // Qt::CustomContextMenu


    // Create the workspace window
    auto* workspace_window = _script_manager->create_object("Editor", editor::script::FactoryUserdata<QScriptedWorkspaceWindow>{ });
    _workspace_window = QWorkspaceWindowPtr{ workspace_window, &detail::custom_qworkspacewindow_deleter };

    // Set the workspace window to the editorSpace layout.
    _ui->editorSpace->setStyleSheet("QMainWindow { border: 1px solid #333; }");
    _ui->editorSpace->layout()->addWidget(_workspace_window.get());


    // Query for types annotated with the 'project' tag.
    auto project_types = _script_manager->query_types("[project] : CProject");
    for (const auto& project_type : project_types)
    {
        _project_factories.emplace(project_type.name(), &detail::scripted_project_factory);
    }
}

void QEditorMainWindow::add_project_factory(const std::string& class_name, editor::project::FactoryFunction factory) noexcept
{
    _project_factories.emplace(class_name, std::move(factory));
}

auto QEditorMainWindow::get_project_factory(const std::string& class_name) const noexcept -> const editor::project::FactoryFunction&
{
    assert(_project_factories.count(class_name) > 0);
    return _project_factories.at(class_name);
}

void QEditorMainWindow::onOpenProject()
{
    // Find a project file
    QString project_file_path = QFileDialog::getOpenFileName(this, tr("Open project file..."), _editor_settings.get("projects.location").toString(), tr("Project files (*.mprj)"));

    // Try to open the project file.
    QFile project_file{ project_file_path };
    project_file.open(QFile::OpenModeFlag::ReadOnly);
    if (!project_file.isOpen())
    {
        return;
    }

    // Load the project document
    auto project_document = QJsonDocument::fromJson(project_file.readAll());
    if (!project_document.isObject())
    {
        return;
    }

    // Find the project class
    QJsonObject project_root = project_document.object();
    QString project_class = project_root.value("class").toString();

    // Call the slot
    open_project({ project_class.toStdString(), "", QFileInfo{ project_file_path } });
}

void QEditorMainWindow::onSaveProject()
{
    if (_active_project != nullptr)
    {
        onSaveProject(_active_project->name());
    }
}

void QEditorMainWindow::onSaveProject(QString name)
{
    auto it = std::find_if(_projects.begin(), _projects.end(), [&name](const QProjectPtr& project)
        {
            return project->name() == name;
        });

    // Save if found
    if (it != _projects.end())
    {
        (*it)->save();
    }
}

void QEditorMainWindow::onCloseProject(QString name)
{
    auto it = std::find_if(_projects.begin(), _projects.end(), [&name](const QProjectPtr& project)
        {
            return project->name() == name;
        });

    // If a project was found erase it.
    if (it != _projects.end())
    {
        _projects.erase(it);
        _active_project = nullptr;
    }

    // Choose a new active project if needed.
    if (!_projects.empty() && !_active_project)
    {
        _active_project = _projects.front().get();
    }
}

void QEditorMainWindow::open_project(const editor::project::FactoryData& data) noexcept
{
    if (_project_factories.count(data.class_name) == 0)
    {
        assert(false);
        return;
    }

    // Get the factory...
    const auto& project_factory = _project_factories.at(data.class_name);

    // Create the project...
    auto* const project = project_factory(_script_manager.get(), data);

    // ???
    if (project)
    {
        project->initialize(this);

        // Profit!
        _projects.emplace_back(project, &detail::custom_qproject_deleter);
        _active_project = project;
    }
}

} // namespace editor
