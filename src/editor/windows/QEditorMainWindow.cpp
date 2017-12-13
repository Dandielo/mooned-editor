#include "QEditorMainWindow.h"
#include "scripted/QScriptedWorkspaceWindow.h"


#include "ui_mainwindow.h"

#include <QMenuBar>

static void initializeEditorMainWindowMenu(QEditorMainWindow* editor, Ui::QTWindow* ui, QMenuBar* menu)
{
    QAction* action = ui->menuEdit->addAction("&Save");
    action->setShortcut(QKeySequence::Save);
    QApplication::connect(action, &QAction::triggered, editor, &QEditorMainWindow::onSave);

    action = ui->menuEdit->addAction("&Open");
    action->setSeparator(QKeySequence::Open);
    QApplication::connect(action, &QAction::triggered, editor, &QEditorMainWindow::onLoad);

    action = ui->menuEdit->addAction("&Export");
    action->setSeparator(QKeySequence::Open);
    QApplication::connect(action, &QAction::triggered, editor, &QEditorMainWindow::onLoad);
}

QEditorMainWindow::QEditorMainWindow()
    : QMainWindow{ }
    , _workspace_window{ nullptr }
    , _script_manager{ nullptr }
    , _project_model{ nullptr }
    , _project{ nullptr }
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
    _project_model = new editor::QProjectModel{};
    window_ui.projectsFileTree->setModel(_project_model);

    QDir projects_path = QDir::homePath() + "/meditor";
    projects_path = projects_path.canonicalPath();
    if (!projects_path.exists())
    {
        QDir(QDir::homePath()).mkdir("meditor");
    }

    _project = new editor::QProject{ "Test", projects_path };
    _project_model->addProject(_project);
}

QEditorMainWindow::~QEditorMainWindow()
{
    reinterpret_cast<QScriptedWorkspaceWindow*>(_workspace_window)->shutdown();
    delete _workspace_window;
    delete _script_manager;
}

void QEditorMainWindow::onSave()
{
    _workspace_window->onSave();
}

void QEditorMainWindow::onLoad()
{
    _workspace_window->onLoad();
}

