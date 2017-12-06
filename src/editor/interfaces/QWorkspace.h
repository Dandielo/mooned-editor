#pragma once 
#include <QString>
#include <QDockWidget>

class QWorkspace : public QDockWidget
{
    Q_OBJECT;
public:
    QWorkspace(QWidget* parent = nullptr);
    virtual ~QWorkspace() override = default;

    virtual QString name() const = 0;
};
