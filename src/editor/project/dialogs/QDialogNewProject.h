#pragma once
#include <QDialog>
#include <QAbstractItemModel>
#include "windows/QEditorMainWindow.h"
#include "project/interfaces/QProject.h"

namespace Ui
{
class DialogCreateProject;
}

namespace editor
{

class QProjectTypesModel : public QAbstractItemModel
{
public:
    QProjectTypesModel(const QVector<QString>& project_types);
    ~QProjectTypesModel();

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QVector<QString> _project_types;
};

class QDialogNewProject : public QDialog
{
    Q_OBJECT;

public:
    QDialogNewProject(QEditorMainWindow* main_window, QWidget* parent = nullptr);
    virtual ~QDialogNewProject() override;

signals:
    void finished(QProject* project);

private slots:
    void checkAndFinish();

private:
    Ui::DialogCreateProject* _ui;

    QMap<QString, SProjectTypeEntry> _projects;

    QProjectTypesModel _model;
};

}
