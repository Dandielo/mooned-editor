#pragma once
#include <QMainWindow>
#include "scripts/CScriptManager.h"

#include "project/interfaces/QProject.h"
#include "project/models/QProjectModel.h"

#include "QWorkspaceWindow.h"

using Scripts::CScriptManager;

using TProjectFactory = editor::QProject*(*)(QString type, QString project_file, void* userdata);

struct SProjectTypeEntry
{
    TProjectFactory factory;
    void* userdata;
};

class QEditorMainWindow : public QMainWindow
{
    Q_OBJECT;

public:
    QEditorMainWindow();
    virtual ~QEditorMainWindow() override;

    QString projectDir();

    void registerProjectType(QString type, TProjectFactory factory, void* userdata = nullptr);

public slots:
    void onSave();
    void onLoad();

    // Project action slots
    void onNewProject();
    void onOpenProject();
    void onSaveProject();

    void onSaveProject(QString name);
    void onCloseProject(QString name);

    QWorkspaceWindow* workspaceWindow() const { return _workspace_window; }
    editor::QProjectModel* projectModel() const { return _project_model; }

private:
    QWorkspaceWindow* _workspace_window;
    CScriptManager* _script_manager;

private:
    editor::QProjectModel* _project_model;

    editor::QProject* _active_project;
    QVector<editor::QProject*> _projects;

    QMap<QString, SProjectTypeEntry> _project_types;
};
