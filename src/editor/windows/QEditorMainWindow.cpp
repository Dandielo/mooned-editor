#include "QEditorMainWindow.h"
#include "scripted/QScriptedWorkspaceWindow.h"
#include "ui_mainwindow.h"

QEditorMainWindow::QEditorMainWindow() : QMainWindow()
{
    Ui::QTWindow window_ui;
    window_ui.setupUi(this);

    // Script classes
    _script_manager = new CScriptManager;
    _script_manager->Initialize("../../src/scripts/main.as");

    // Create the workspace window
    auto* workspace_window = _script_manager->CreateObject<QScriptedWorkspaceWindow, 1>("Editor");
    workspace_window->initialize(_script_manager);
    _workspace_window = workspace_window;

    // Apply the default workspace
    window_ui.editorSpace->setStyleSheet("QMainWindow { border: 1px solid #333; }");
    window_ui.editorSpace->layout()->addWidget(_workspace_window);
}

QEditorMainWindow::~QEditorMainWindow()
{
    reinterpret_cast<QScriptedWorkspaceWindow*>(_workspace_window)->shutdown();
    delete _workspace_window;
    delete _script_manager;
}

