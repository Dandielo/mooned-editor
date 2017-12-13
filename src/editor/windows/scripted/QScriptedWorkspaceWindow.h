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
    QScriptedWorkspaceWindow(asIScriptObject* obj);
    virtual ~QScriptedWorkspaceWindow() override;

    void initialize(Scripts::CScriptManager* script_manager);
    void shutdown();

    virtual void addWorkspace(QWorkspace* workspace) override;
    virtual QWorkspace* activeWorkspace() override;

public slots:
    virtual void onSave() override;
    virtual void onLoad() override;

private:
    Scripts::CScriptManager* _script_manager;
    QWorkspace* _workspace;

private:
    editor::QGraphSerializer* _default_serializer;
};