#pragma once
#include <QDialog>
#include <QAbstractItemModel>
#include "windows/QEditorMainWindow.h"
#include "project/interfaces/QProject.h"

namespace Ui
{

//! The create project dialog UI.
class DialogCreateProject;

}

namespace editor
{

class QProjectTypesModel : public QAbstractItemModel
{
public:
    QProjectTypesModel(const std::map<std::string, project::FactoryFunction>& project_types);
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

//! Dialog opened when creating new projects.
class QDialogNewProject : public QDialog
{
    Q_OBJECT;

public:
    //! The main window widget.
    QDialogNewProject(QEditorMainWindow* main_window);

signals:
    //! Sends a signal with the data for the project factory.
    //! \note Sent on dialog success.
    void finished(const editor::project::FactoryData&) const;

    //! Sends a signal with no data indicating, project creation was aborted.
    void canceled() const;

protected slots:
    //! Called when the dialog was exited in any possible way.
    void checkAndFinish() noexcept;

private:
    //! The dialog UI representation;
    std::unique_ptr<Ui::DialogCreateProject> _ui;

    //! The project model.
    QProjectTypesModel _model;
};

}
