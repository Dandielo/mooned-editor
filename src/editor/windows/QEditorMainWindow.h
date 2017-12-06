#pragma once 
#include <QMainWindow>
#include "scripts/CScriptManager.h"

#include "QWorkspaceWindow.h"

using Scripts::CScriptManager;

class QEditorMainWindow : public QMainWindow
{
    Q_OBJECT;

public:
    QEditorMainWindow();
    virtual ~QEditorMainWindow() override;

private:
    QWorkspaceWindow* _workspace_window;
    CScriptManager* _script_manager;
};
