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

public slots:
    virtual void onSave() = 0;
    virtual void onLoad() = 0;
};