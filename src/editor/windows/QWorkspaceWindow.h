#pragma once 
#include <QMainWindow>
#include "interfaces/QWorkspace.h"

class QWorkspaceWindow : public QMainWindow
{
    Q_OBJECT;
public:
    QWorkspaceWindow() = default;
    virtual ~QWorkspaceWindow() override = default;

    virtual void addWorkspace(QWorkspace* workspace) = 0;
    virtual QWorkspace* activeWorkspace() = 0;
};