#pragma once
#include <QVector>
#include <QAbstractItemModel>
#include "QProjectContextMenuHelper.h"

namespace editor
{
    class QGraph;
    class QProject;
    class QProjectTree;

    class QProjectModel : public QAbstractItemModel
    {
        Q_OBJECT;

    public:
        QProjectModel(QTreeView* tree_view, QObject* parent = nullptr);
        virtual ~QProjectModel() override;

        void addProject(QProjectTree* project);
        void removeProject(QProjectTree* project);

        virtual int rowCount(const QModelIndex &parent) const override;
        virtual int columnCount(const QModelIndex &parent) const override;

        virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
        virtual QModelIndex parent(const QModelIndex &child) const override;

        virtual QVariant data(const QModelIndex &index, int role) const override;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        QProjectContextMenuHelper* contextMenuHelper() { return &_context_menu_helper; }

    public slots:
        void projectTreeChanged(QProjectTree* tree);

    private:
        QVector<QString> _sections;
        QVector<QProjectTree*> _projects;

        QProjectContextMenuHelper _context_menu_helper;
    };
}
