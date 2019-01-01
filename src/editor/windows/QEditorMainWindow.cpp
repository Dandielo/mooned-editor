#include "QEditorMainWindow.h"
#include "scripted/QScriptedWorkspaceWindow.h"
#include "project/scripted/QScriptedProject.h"
#include "project/dialogs/QDialogNewProject.h"

#include "ui_mainwindow.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

static void destroyProject(editor::QProject* project)
{
    auto* scripted_project = dynamic_cast<editor::QScriptedProject*>(project);
    if (nullptr != scripted_project)
    {
        scripted_project->save();
        scripted_project->shutdown();
    }
    delete scripted_project;
}

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

static editor::QProject* scriptedProjectFactory(const QString& type, const QString& name, const QString& file_path, Scripts::CScriptManager* script_manager)
{
    auto factory_userdata = ::editor::QScriptedProject::FactoryData{ QFileInfo{ file_path }, type };

    // Create the project object
    auto* project = script_manager->create_object(type.toStdString(), factory_userdata);
    project->setScriptManager(script_manager);
    project->load();

    return project;
}

static void initializeEditorMainWindowMenu(QEditorMainWindow* editor, Ui::QTWindow* ui, QMenuBar* menu)
{
    QAction* action = ui->menuEdit->addAction("&Save");
    action->setShortcut(QKeySequence::Save);
    QApplication::connect(action, &QAction::triggered, editor, &QEditorMainWindow::onSave);

    action = ui->menuEdit->addAction("&Open");
    action->setShortcut(QKeySequence::Open);
    QApplication::connect(action, &QAction::triggered, editor, &QEditorMainWindow::onLoad);

    action = ui->menuEdit->addAction("&Export");
    QApplication::connect(action, &QAction::triggered, editor, &QEditorMainWindow::onLoad);

    // Project actions
    action = ui->menuFile->addAction("&New project");
    QApplication::connect(action, &QAction::triggered, editor, [editor]()
        {
            auto* dialog = new editor::QDialogNewProject{ editor };
            QApplication::connect(dialog, &editor::QDialogNewProject::finished, editor, &QEditorMainWindow::onNewProject);
            dialog->show();
        });
    action = ui->menuFile->addAction("&Open project");
    QApplication::connect(action, &QAction::triggered, editor, &QEditorMainWindow::onOpenProject);
    action = ui->menuFile->addAction("&Save project");
    QApplication::connect(action, &QAction::triggered, editor, (void(QEditorMainWindow::*)())&QEditorMainWindow::onSaveProject);
}

QEditorMainWindow::QEditorMainWindow()
    : QMainWindow{ }
    , _workspace_window{ nullptr }
    , _script_manager{ nullptr }
    , _project_model{ nullptr }
    , _active_project{ nullptr }
    , _projects{ }
{
    Ui::QTWindow window_ui;
    window_ui.setupUi(this);

    // Script classes
    _script_manager = new CScriptManager;
    extern void registerEditorInterface(asIScriptEngine* engine);
    asIScriptEngine* script_engine = _script_manager->engine().native();
    registerEditorInterface(script_engine);

    // Load all script project types
    _script_manager->Initialize("../../src/scripts/main.as");
    auto project_types = _script_manager->QueryTypes("[project] : CProject");
    for (const auto& project_type : project_types)
    {
        registerProjectType(project_type.name().c_str(), reinterpret_cast<editor::TProjectFactory>(scriptedProjectFactory), _script_manager);
    }

    // Create the workspace window
    auto* workspace_window = _script_manager->CreateObject<QScriptedWorkspaceWindow, 1>("Editor");
    workspace_window->initialize(_script_manager);
    _workspace_window = workspace_window;

    // Apply the default workspace
    window_ui.editorSpace->setStyleSheet("QMainWindow { border: 1px solid #333; }");
    window_ui.editorSpace->layout()->addWidget(_workspace_window);

    // Setup the menu bar
    initializeEditorMainWindowMenu(this, &window_ui, window_ui.menuBar);

    // Setup other UI elements
    _project_model = new editor::QProjectModel{ window_ui.projectsFileTree };
    _project_model->context_menu_helper().initialize(this);

    window_ui.projectsFileTree->setModel(_project_model);
    window_ui.projectsFileTree->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(window_ui.projectsFileTree, &QTreeView::customContextMenuRequested, &_project_model->context_menu_helper(), &editor::QProjectContextMenuHelper::onCustomContextMenuAction);
}

QEditorMainWindow::~QEditorMainWindow()
{
    for (auto* prj : _projects)
    {
        destroyProject(prj);
    }

    reinterpret_cast<QScriptedWorkspaceWindow*>(_workspace_window)->shutdown();
    delete _project_model;
    delete _workspace_window;
    delete _script_manager;
}

QString QEditorMainWindow::defalutProjectLocation()
{
    QDir projects_path = QDir::homePath() + "/meditor";
    projects_path = projects_path.canonicalPath();

    if (!projects_path.exists())
    {
        QDir(QDir::homePath()).mkdir("meditor");
    }

    return projects_path.canonicalPath();
}

void QEditorMainWindow::registerProjectType(QString type, editor::TProjectFactory factory, void* userdata)
{
    _project_types.insert(type, { factory, userdata });
}

void QEditorMainWindow::onSave()
{
    //_workspace_window->onSave();
}

void QEditorMainWindow::onLoad()
{
    //_workspace_window->onLoad();
}

void QEditorMainWindow::onNewProject(editor::QProject* project)
{
    project->initialize(this);

    _active_project = project;
    _projects.append(_active_project);;
}

void QEditorMainWindow::onOpenProject()
{
    // Find a project file
    QString project_file_path = QFileDialog::getOpenFileName(this, tr("Open project file..."), defalutProjectLocation(), tr("Project files (*.mprj)"));

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

    if (!_project_types.contains(project_class))
    {
        return;
    }

    // Create the given project from a registered factory
    const auto& entry = _project_types[project_class];
    editor::QProject* const project = (entry.factory)(project_class, "", project_file_path, entry.userdata);

    // Initialize and register
    if (project)
    {
        project->initialize(this);

        _active_project = project;
        _projects.append(_active_project);
    }
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
    auto* project = findProjectByName(_projects, name);
    if (project != nullptr)
    {
        project->save();
    }
}

void QEditorMainWindow::onCloseProject(QString name)
{
    auto* project = findProjectByName(_projects, name);
    if (project != nullptr)
    {
        _projects.removeAt(_projects.indexOf(project));

        if (project == _active_project)
        {
            if (_projects.isEmpty())
            {
                _active_project = nullptr;
            }
            else
            {
                _active_project = _projects.last();
            }
        }

        destroyProject(project);
    }
}

