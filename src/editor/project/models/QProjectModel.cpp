#include "QProjectModel.h"

#include "project/interfaces/QProject.h"

#include "project/models/QProjectTreeNode.h"
#include "project/models/QProjectTree.h"

static editor::QProjectTree* find(QVector<editor::QProjectTree*>& projects, editor::QProject* to_find)
{
    editor::QProjectTree* tree = nullptr;
    for (editor::QProjectTree* prj : projects)
    {
        if (prj->project() == to_find)
        {
            tree = prj;
            break;
        }
    }
    return tree;
}

static bool contains(QVector<editor::QProjectTree*>& projects, editor::QProject* to_find)
{
    return find(projects, to_find) != nullptr;
}

editor::QProjectModel::QProjectModel(QObject* parent /* = nullptr */)
    : QAbstractItemModel{ parent }
    , _projects{ }
{
    _sections << "Name";
}

editor::QProjectModel::~QProjectModel()
{
}

void editor::QProjectModel::addProject(QProject* project)
{
    if (!contains(_projects, project))
    {
        _projects.append(new editor::QProjectTree{ project });
    }

    layoutChanged();
}

void editor::QProjectModel::removeProject(QProject* project)
{
    if (contains(_projects, project))
    {
        _projects.remove(_projects.indexOf(find(_projects, project)));
    }

    layoutChanged();
}

int editor::QProjectModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        return _projects.count();
    }
    else
    {
        return static_cast<QProjectTreeNode*>(parent.internalPointer())->childCount();
    }
}

int editor::QProjectModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return 1;
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
        parent_node = static_cast<QProjectTreeNode*>(parent.internalPointer());
    }

    QProjectTreeNode* child_node = parent_node->child(row);
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

    auto* child_node = static_cast<QProjectTreeNode*>(child.internalPointer());
    auto* parent_node = child_node->parent();

    if (parent_node == nullptr)
    {
        return { };
    }

    return createIndex(parent_node->index(), 0, parent_node);
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

    auto* node = static_cast<QProjectTreeNode*>(index.internalPointer());
    return { node->toString() };
}

QVariant editor::QProjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _sections.at(section);

    return { };
}
