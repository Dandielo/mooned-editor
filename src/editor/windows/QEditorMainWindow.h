#pragma once
#include <QMainWindow>
#include "scripts/CScriptManager.h"

#include "project/interfaces/QProject.h"
#include "project/models/QProjectModel.h"

#include "QWorkspaceWindow.h"

using Scripts::CScriptManager;

class QEditorMainWindow : public QMainWindow
{
    Q_OBJECT;

public:
    QEditorMainWindow();
    virtual ~QEditorMainWindow() override;

public slots:
    void onSave();
    void onLoad();

private:
    QWorkspaceWindow* _workspace_window;
    CScriptManager* _script_manager;

private:
    editor::QProjectModel* _project_model;
    editor::QProject* _project;
};
