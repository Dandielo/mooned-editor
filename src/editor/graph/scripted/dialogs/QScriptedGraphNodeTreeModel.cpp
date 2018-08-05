#include "QScriptedGraphNodeTreeModel.h"
#include "scripts/CScriptManager.h"

#include <angelscript.h>
#include <cassert>

editor::QScriptedGraphNodeTreeModel::QScriptedGraphNodeTreeModel(const Scripts::CScriptManager* script_manager, QVector<asITypeInfo*> types)
    : QAbstractItemModel{ nullptr }
    , _script_manager{ script_manager }
    , _types{ types }
{
    _sections << "Name" << "Description" << "Type";
}

editor::QScriptedGraphNodeTreeModel::~QScriptedGraphNodeTreeModel()
{
}

int editor::QScriptedGraphNodeTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        return _types.count();
    }

    return 0;
}

int editor::QScriptedGraphNodeTreeModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QModelIndex editor::QScriptedGraphNodeTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return {};
    }

    assert(!parent.isValid()); // For now we can't have a parent
    return createIndex(row, column, _types.at(row));
}

QModelIndex editor::QScriptedGraphNodeTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
    {
        return {};
    }
    return {};
}

QVariant editor::QScriptedGraphNodeTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }

    auto* type_info = static_cast<asITypeInfo*>(index.internalPointer());

    if (role == Qt::UserRole)
    {
        return { type_info->GetName() };
    }

    if (role != Qt::DisplayRole)
    {
        return {};
    }

    QVariant result;
    if (index.column() == 1)
    {
        result = QString::fromStdString(_script_manager->GetTypeAttr(type_info, "desc", "{none}"));
    }
    else if (index.column() == 2)
    {
        result = type_info->GetName();
    }
    else
    {
        result = QString::fromStdString(_script_manager->GetTypeAttr(type_info, "name", "{none}"));
    }

    return result;
}

QVariant editor::QScriptedGraphNodeTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _sections.at(section);

    return {};
}
