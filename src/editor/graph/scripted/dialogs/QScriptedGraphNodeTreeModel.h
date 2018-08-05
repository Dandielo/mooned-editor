#pragma once
#include <QAbstractItemModel>

class asITypeInfo;

namespace Scripts
{
class CScriptManager;
}

namespace editor
{
    class QScriptedGraphNodeTreeModel : public QAbstractItemModel
    {
        Q_OBJECT;

    public:
        QScriptedGraphNodeTreeModel(const Scripts::CScriptManager* script_manager, QVector<asITypeInfo*> types);
        virtual ~QScriptedGraphNodeTreeModel() override;

        virtual int rowCount(const QModelIndex &parent) const override;
        virtual int columnCount(const QModelIndex &parent) const override;

        virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
        virtual QModelIndex parent(const QModelIndex &child) const override;

        virtual QVariant data(const QModelIndex &index, int role) const override;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    private:
        const Scripts::CScriptManager* const _script_manager;

        QVector<QString> _sections;
        QVector<asITypeInfo*> _types;
    };
}
