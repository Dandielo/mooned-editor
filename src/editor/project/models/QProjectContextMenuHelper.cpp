#include "QProjectContextMenuHelper.h"
#include "QProjectTreeNode.h"
#include "QProjectTree.h"

#include "windows/QEditorMainWindow.h"

#include <QModelIndex>
#include <QMenu>

//#todo create a metatype header
Q_DECLARE_METATYPE(editor::QProjectTreeNode*);

editor::QProjectContextMenuHelper::QProjectContextMenuHelper(QTreeView* tree_view, QObject* parent /* = nullptr */)
    : QObject{ parent }
    , _tree_view{ tree_view }
    , _menu{ nullptr }
{
    _menu = new QMenu();
}

editor::QProjectContextMenuHelper::~QProjectContextMenuHelper()
{
    delete _menu;
}

void editor::QProjectContextMenuHelper::initialize(QEditorMainWindow* mwindow)
{
    QAction* action;

    auto add_project_action = [&](QString text)
    {
        action = _menu->addAction(text);
        action->setObjectName("project." + text.toLower());
        connect(action, &QAction::triggered, this, &QProjectContextMenuHelper::onContextMenuAction);
    };

    add_project_action("Save");
    add_project_action("Close");
    _menu->addSeparator();
    add_project_action("New graph");

    initializeProjectActions(mwindow);
}

void editor::QProjectContextMenuHelper::initializeProjectActions(QEditorMainWindow* mwindow)
{
    connect(this, &QProjectContextMenuHelper::closeProject, mwindow, &QEditorMainWindow::onCloseProject);
    connect(this, &QProjectContextMenuHelper::saveProject, mwindow, (void(QEditorMainWindow::*)(QString)) &QEditorMainWindow::onSaveProject);
}

void editor::QProjectContextMenuHelper::onProjectContextMenu(const QPoint& pos)
{
    QModelIndex index = _tree_view->indexAt(pos);
    if (index.isValid())
    {
        auto* node = reinterpret_cast<QProjectTreeNode*>(index.internalPointer());

        if (node != nullptr)
        {
            _menu->setProperty("node", QVariant::fromValue(node));
            _menu->exec(_tree_view->mapToGlobal(pos));
        }
    }
}

void editor::QProjectContextMenuHelper::onContextMenuAction()
{
    auto* node = qvariant_cast<QProjectTreeNode*>(_menu->property("node"));
    auto action_name = sender()->objectName();

    auto emit_if_action = [&](QString name, auto fn) {
        if (action_name == name)
        {
            fn(node->toString());
        }
    };

    emit_if_action("project.close", [&](QString node_name) {
        emit closeProject(node_name);
    });
    emit_if_action("project.save", [&](QString node_name) {
        emit saveProject(node_name);
    });
}
