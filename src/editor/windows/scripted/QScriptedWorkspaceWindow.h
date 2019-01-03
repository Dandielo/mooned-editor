#pragma once
#include "windows/QWorkspaceWindow.h"

#include "scripts/CNativeScriptObject.h"
#include "scripts/CScriptManager.h"

#include "graph/interfaces/QGraphSerializer.h"

class QScriptedWorkspaceWindow : public QWorkspaceWindow, public Scripts::CNativeScriptObject<QScriptedWorkspaceWindow>
{
    Q_OBJECT;
    M_SCRIPT_TYPE(QScriptedWorkspaceWindow, "CWorkspaceWindow");

public:
    //! Alias for the script factory structure for this type.
    using FactoryData = editor::script::FactoryUserdata<QScriptedWorkspaceWindow>;

public:
    QScriptedWorkspaceWindow(editor::script::ScriptObject&& obj);
    virtual ~QScriptedWorkspaceWindow() override;

    void initialize(Scripts::CScriptManager* script_manager);
    void shutdown();

    virtual void addWorkspace(QWorkspace* workspace) override;

public slots:
    void createWorkspace();
    void closeWorkspace(QString name);

protected:
    void setActiveWorkspace(QWorkspace* workspace);

private:
    Scripts::CScriptManager* _script_manager;

    QWorkspace* _active_workspace;
    QVector<QWorkspace*> _workspaces;
    QVector<QString> _workspace_types;
};