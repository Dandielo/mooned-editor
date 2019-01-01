#include <project/models/QProjectModel.h>
#include <project/models/QProjectTreeNode.h>
#include <project/models/QProjectTree.h>

#include <project/interfaces/QProject.h>

#include <algorithm>

//static editor::QProjectTree* find(QVector<editor::QProjectTree*>& projects, editor::QProject* to_find)
//{
//    editor::QProjectTree* tree = nullptr;
//    for (editor::QProjectTree* prj : projects)
//    {
//        if (prj->project() == to_find)
//        {
//            tree = prj;
//            break;
//        }
//    }
//    return tree;
//}
//
//static bool contains(QVector<editor::QProjectTree*>& projects, editor::QProject* to_find)
//{
//    return find(projects, to_find) != nullptr;
//}

namespace editor
{

editor::QProjectModel::QProjectModel(QTreeView* tree_view, QObject* parent /* = nullptr */)
    : QAbstractItemModel{ parent }
    , _context_menu_helper{ tree_view, this }
{
    _sections << "Name";
}

void editor::QProjectModel::add_project(const std::unique_ptr<ProjectTreeRoot>& project) noexcept
{
    auto project_it = std::find(_projects.begin(), _projects.end(), project.get());
    if (project_it == _projects.end())
    {
        int project_count = static_cast<int>(_projects.size());

        beginInsertRows({}, project_count, project_count);

        _projects.push_back(project.get());

        endInsertRows();
    }
}

void editor::QProjectModel::remove_project(const std::unique_ptr<ProjectTreeRoot>& project) noexcept
{
    auto project_it = std::find(_projects.begin(), _projects.end(), project.get());
    if (project_it != _projects.end())
    {
        auto index = std::distance(_projects.begin(), project_it);

        beginRemoveRows({}, index, index);

        _projects.erase(project_it);

        endRemoveRows();
    }
}

int editor::QProjectModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        return static_cast<int>(_projects.size());
    }
    else
    {
        return reinterpret_cast<ProjectTreeNode*>(parent.internalPointer())->child_count();
    }
}

int editor::QProjectModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return _sections.size();
}

QModelIndex editor::QProjectModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex()*/) const
{
    if (!hasIndex(row, column, parent))
    {
        return { };
    }

    ProjectTreeNode* parent_node = nullptr;
    if (!parent.isValid())
    {
        return createIndex(row, column, _projects.at(row));
    }
    else
    {
        parent_node = reinterpret_cast<ProjectTreeNode*>(parent.internalPointer());
    }

    ProjectTreeNode* child_node = parent_node->child(row);
    if (child_node)
    {
        return createIndex(row, column, child_node);
    }
    else
    {
        return { };
    }
}

QModelIndex editor::QProjectModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
    {
        return { };
    }

    auto* child_node = reinterpret_cast<ProjectTreeNode*>(child.internalPointer());
    auto* parent_node = child_node->parent();

    if (parent_node == nullptr)
    {
        return { };
    }

    return createIndex(static_cast<int>(parent_node->index()), 0, parent_node);
}

QVariant editor::QProjectModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    if (!index.isValid())
    {
        return { };
    }

    if (role != Qt::DisplayRole)
    {
        return { };
    }

    auto* node = reinterpret_cast<ProjectTreeNode*>(index.internalPointer());
    return { node->value(Qt::DisplayRole) };
}

QVariant editor::QProjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _sections.at(section);

    return { };
}

void editor::QProjectModel::projectTreeChanged(ProjectTreeRoot* /*root_node*/)
{
    layoutChanged();
}

} // namespace editor
