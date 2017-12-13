#pragma once
#include <QVector>
#include <QAbstractItemModel>

namespace editor
{
    class QProject;
    class QProjectTree;

    class QProjectModel : public QAbstractItemModel
    {
        Q_OBJECT;

    public:
        QProjectModel(QObject* parent = nullptr);
        virtual ~QProjectModel() override;

        void addProject(QProject* project);
        void removeProject(QProject* project);

        virtual int rowCount(const QModelIndex &parent) const override;
        virtual int columnCount(const QModelIndex &parent) const override;

        virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
        virtual QModelIndex parent(const QModelIndex &child) const override;

        virtual QVariant data(const QModelIndex &index, int role) const override;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    private:
        QVector<QString> _sections;
        QVector<QProjectTree*> _projects;
    };
}
