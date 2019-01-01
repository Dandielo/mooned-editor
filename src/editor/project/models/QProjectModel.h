#pragma once
#include <project/models/QProjectContextMenuHelper.h>

#include <QAbstractItemModel>
#include <QVector>

#include <vector>
#include <memory>

namespace editor
{

class QGraph;
class QProject;

class ProjectTreeRoot;

//! Implements the QAbstractItemModel to visualize project trees in a QTreeView.
//! \note This model allows to visualize more than one tree at a time.
class QProjectModel : public QAbstractItemModel
{
    Q_OBJECT;

public:
    //! Create a new model for the given tree view and parent.
    QProjectModel(QTreeView* tree_view, QObject* parent = nullptr);

    //! Adds a project tree to the model.
    void add_project(const std::unique_ptr<ProjectTreeRoot>& project) noexcept;

    //! Removes a project tree from the model.
    void remove_project(const std::unique_ptr<ProjectTreeRoot>& project) noexcept;

    //! \returns The context menu helper object of this model.
    auto context_menu_helper() noexcept -> QProjectContextMenuHelper& { return _context_menu_helper; }
    auto context_menu_helper() const noexcept -> const QProjectContextMenuHelper& { return _context_menu_helper; }

protected:
    //////////////////////////////////////////////////////////////////////////
    // Implementation of the QAbstractItemModel interface.

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;


public slots:
    //////////////////////////////////////////////////////////////////////////
    // Definition of additional slots available to connect with this project.
    void projectTreeChanged(ProjectTreeRoot* tree);

private:
    //! List of sections to be displayed in this model.
    QVector<QString> _sections{ };

    //! List of all projects to be displayed in this model.
    std::vector<ProjectTreeRoot*> _projects{ };

    //! Helper object with contextual actions.
    QProjectContextMenuHelper _context_menu_helper;
};

} // namespace editor
