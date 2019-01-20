#include <project/models/QProjectModel.h>
#include <project/interfaces/QProject.h>

#include <algorithm>

namespace editor
{

QProjectModel::QProjectModel() noexcept
    : QAbstractItemModel{ nullptr }
    , _context_menu_helper{ nullptr, this }
{
    _sections << "Name";
}

void QProjectModel::add_project(QProject* project) noexcept
{
    auto project_it = std::find(_projects.begin(), _projects.end(), project);
    if (project_it == _projects.end())
    {
        int project_count = static_cast<int>(_projects.size());

        beginInsertRows({}, project_count, project_count);

        _projects.push_back(project);

        endInsertRows();
    }
}

void editor::QProjectModel::remove_project(QProject* project) noexcept
{
    auto project_it = std::find(_projects.begin(), _projects.end(), project);
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
        return reinterpret_cast<QProjectTreeNode*>(parent.internalPointer())->child_nodes().size();
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

    QProjectTreeNode* parent_node = nullptr;
    if (!parent.isValid())
    {
        return createIndex(row, column, _projects.at(row));
    }
    else
    {
        parent_node = reinterpret_cast<QProjectTreeNode*>(parent.internalPointer());
    }

    QProjectTreeNode* child_node = parent_node->child_nodes().at(row);
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

    auto* cnode = reinterpret_cast<QProjectTreeNode*>(child.internalPointer());
    auto* pnode = cnode->parent_node();

    if (pnode == nullptr)
    {
        return { };
    }

    int pindex = 0;
    if (pnode->parent_node() != nullptr)
    {
        pindex = pnode->parent_node()->child_nodes().indexOf(pnode);
    }
    else
    {
        auto it = std::find_if(_projects.begin(), _projects.end(), [pnode](QProject* project)
            {
                return pnode == project;
            });

        pindex = static_cast<int>(std::distance(_projects.begin(), it));
    }

    return createIndex(pindex, 0, pnode);
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

    auto* node = reinterpret_cast<QProjectTreeNode*>(index.internalPointer());
    return { node->value(Qt::DisplayRole) };
}

QVariant editor::QProjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _sections.at(section);

    return { };
}

void editor::QProjectModel::projectTreeChanged(QProjectTreeNode* /*root_node*/)
{
    layoutChanged();
}

} // namespace editor
