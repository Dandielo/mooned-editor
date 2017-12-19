#pragma once
#include <QObject>
#include <QTreeView>
#include <QMenu>

class QEditorMainWindow;

namespace editor
{
    class QProjectContextMenuHelper : public QObject
    {
        Q_OBJECT;

    public:
        QProjectContextMenuHelper(QTreeView* tree_view, QObject* parent = nullptr);
        virtual ~QProjectContextMenuHelper() override;

        void initialize(QEditorMainWindow* mwindow);

    protected:
        void initializeProjectActions(QEditorMainWindow* mwindow);

    signals:
        void saveProject(QString name);
        void closeProject(QString name);

    public slots:
        void onCustomContextMenuAction(const QPoint& pos);
        void onMouseDoubleClickAction(const QModelIndex& index);
        void onProjectMenuAction();
        void onNodeMenuAction();

    private:
        QTreeView* _tree_view;
        QMenu* _project_menu;
        QMenu* _node_menu;
    };
}
