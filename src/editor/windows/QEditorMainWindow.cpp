#include "QEditorMainWindow.h"
#include "scripted/QScriptedWorkspaceWindow.h"
#include "project/scripted/QScriptedProject.h"

#include "ui_mainwindow.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>

static void destroyProject(editor::QProject* project)
{
    auto* scripted_project = dynamic_cast<editor::QScriptedProject*>(project);
    if (nullptr != scripted_project)
    {
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
    QApplication::connect(action, &QAction::triggered, editor, &QEditorMainWindow::onNewProject);
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
    , _projects{ }
{
    Ui::QTWindow window_ui;
    window_ui.setupUi(this);

    // Script classes
    _script_manager = new CScriptManager;
    extern void registerEditorInterface(asIScriptEngine* engine);
    asIScriptEngine* script_engine = *_script_manager->interpreter();
    registerEditorInterface(script_engine);

    _script_manager->Initialize("../../src/scripts/main.as");

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
    _project_model->contextMenuHelper()->initialize(this);

    window_ui.projectsFileTree->setModel(_project_model);
    window_ui.projectsFileTree->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(window_ui.projectsFileTree, &QTreeView::customContextMenuRequested, _project_model->contextMenuHelper(), &editor::QProjectContextMenuHelper::onProjectContextMenu);
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

QString QEditorMainWindow::projectDir()
{
    QDir projects_path = QDir::homePath() + "/meditor";
    projects_path = projects_path.canonicalPath();

    if (!projects_path.exists())
    {
        QDir(QDir::homePath()).mkdir("meditor");
    }

    return projects_path.canonicalPath();
}

void QEditorMainWindow::onSave()
{
    //_workspace_window->onSave();
}

void QEditorMainWindow::onLoad()
{
    //_workspace_window->onLoad();
}

void QEditorMainWindow::onNewProject()
{
    //auto* project = new editor::QScriptedProject{ "Test", projectDir() };
    //project->initialize(_script_manager);
    //project->initialize(this);

    //_active_project = project;
    //_projects.append(_active_project);;
}

void QEditorMainWindow::onOpenProject()
{
    auto project = new editor::QScriptedProject{ };

    // Find a project file
    QString file_path = QFileDialog::getOpenFileName(this, "Open project file...", projectDir());
    project->open({ file_path });

    if (project->isValid())
    {
        project->initialize(_script_manager);
        project->initialize(this);

        // Add the project (if valid)
        _active_project = project;
        _projects.append(_active_project);
    }
    else
    {
        delete project;
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

