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

    QString defalutProjectLocation();

    void registerProjectType(QString type, editor::TProjectFactory factory, void* userdata = nullptr);

    QWorkspaceWindow* workspaceWindow() const { return _workspace_window; }
    editor::QProjectModel* projectModel() const { return _project_model; }

    //! Returns all registered project type names.
    auto getProjectTypes() const -> const auto& { return _project_types; }

public slots:
    void onSave();
    void onLoad();

    // Project action slots
    void onNewProject(editor::QProject* project);
    void onOpenProject();
    void onSaveProject();

    void onSaveProject(QString name);
    void onCloseProject(QString name);

private:
    QWorkspaceWindow* _workspace_window;
    CScriptManager* _script_manager;

private:
    editor::QProjectModel* _project_model;

    editor::QProject* _active_project;
    QVector<editor::QProject*> _projects;

    QMap<QString, editor::SProjectTypeEntry> _project_types;
};
