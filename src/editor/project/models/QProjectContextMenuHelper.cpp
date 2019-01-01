#include "QProjectContextMenuHelper.h"
#include "QProjectTreeNode.h"
#include "QProjectTree.h"

#include "project/interfaces/QProject.h"
#include "project/dialogs/QDialogNewProjectGraph.h"

#include "windows/QEditorMainWindow.h"

#include <QModelIndex>
#include <QMenu>

//#todo create a metatype header
Q_DECLARE_METATYPE(editor::ProjectTreeNode*);
Q_DECLARE_METATYPE(editor::ProjectTreeRoot*);

editor::QProjectContextMenuHelper::QProjectContextMenuHelper(QTreeView* tree_view, QObject* parent /* = nullptr */)
    : QObject{ parent }
    , _tree_view{ tree_view }
    , _project_menu{ nullptr }
    , _node_menu{ nullptr }
{
    _project_menu = new QMenu{};
    _node_menu = new QMenu{};
}

editor::QProjectContextMenuHelper::~QProjectContextMenuHelper()
{
    delete _node_menu;
    delete _project_menu;
}

void editor::QProjectContextMenuHelper::initialize(QEditorMainWindow* mwindow)
{
    QAction* action;

    auto add_project_action = [&](QString text)
    {
        action = _project_menu->addAction(text);
        action->setObjectName("project." + text.toLower());
        connect(action, &QAction::triggered, this, &QProjectContextMenuHelper::onProjectMenuAction);
    };

    add_project_action("Save");
    add_project_action("Close");
    _project_menu->addSeparator();
    add_project_action("New graph");

    initializeProjectActions(mwindow);


    auto add_node_action = [&](QString text)
    {
        action = _node_menu->addAction(text);
        action->setObjectName("node." + text.toLower());
        connect(action, &QAction::triggered, this, &QProjectContextMenuHelper::onNodeMenuAction);
    };

    add_node_action("Open");
    add_node_action("Save");
    add_node_action("Export");
    add_node_action("Delete");

    connect(_tree_view, &QTreeView::doubleClicked, this, &QProjectContextMenuHelper::onMouseDoubleClickAction);
}

void editor::QProjectContextMenuHelper::initializeProjectActions(QEditorMainWindow* mwindow)
{
    connect(this, &QProjectContextMenuHelper::closeProject, mwindow, &QEditorMainWindow::onCloseProject);
    connect(this, &QProjectContextMenuHelper::saveProject, mwindow, (void(QEditorMainWindow::*)(QString)) &QEditorMainWindow::onSaveProject);
}

void editor::QProjectContextMenuHelper::onCustomContextMenuAction(const QPoint& pos)
{
    QModelIndex index = _tree_view->indexAt(pos);
    if (index.isValid())
    {
        auto* node = reinterpret_cast<ProjectTreeNode*>(index.internalPointer());
        auto* tree = dynamic_cast<ProjectTreeRoot*>(node);

        if (tree != nullptr)
        {
            _project_menu->setProperty("tree", QVariant::fromValue(tree));
            _project_menu->exec(_tree_view->mapToGlobal(pos));
        }
        else if (node != nullptr)
        {
            _node_menu->setProperty("node", QVariant::fromValue(node));
            _node_menu->exec(_tree_view->mapToGlobal(pos));
        }
    }
}

void editor::QProjectContextMenuHelper::onMouseDoubleClickAction(const QModelIndex& index)
{
    if (index.isValid())
    {
        auto* node = reinterpret_cast<ProjectTreeNode*>(index.internalPointer());
        auto* tree = dynamic_cast<ProjectTreeRoot*>(node->parent());

        if (nullptr != tree)
        {
            tree->project()->openElement(node->value(Qt::ItemDataRole::DisplayRole).toString());
        }
    }
}

void editor::QProjectContextMenuHelper::onProjectMenuAction()
{
    auto* tree = qvariant_cast<ProjectTreeRoot*>(_project_menu->property("tree"));

    auto action_name = sender()->objectName();

    auto emit_if_action = [&](QString name, auto fn) {
        if (action_name == name)
        {
            fn(tree->value(Qt::ItemDataRole::DisplayRole).toString());
        }
    };

    if (tree)
    {
        emit_if_action("project.close", [&](QString node_name) {
            emit closeProject(node_name);
        });
        emit_if_action("project.save", [&](QString node_name) {
            emit saveProject(node_name);
        });
        emit_if_action("project.new graph", [&](QString node_name) {
            auto* project = tree->project();
            auto* dialog = new QDialogNewProjectGraph{ project };

            connect(dialog, &QDialogNewProjectGraph::finished, [&, project](const QString& name) {
                project->newGraph("ExampleGraph", name);
            });

            dialog->show();
        });
    }
}

void editor::QProjectContextMenuHelper::onNodeMenuAction()
{
    auto* node = qvariant_cast<ProjectTreeNode*>(_node_menu->property("node"));
    auto* tree = dynamic_cast<ProjectTreeRoot*>(node->parent());

    auto action_name = sender()->objectName();

    auto emit_if_action = [&](QString name, auto fn) {
        if (action_name == name)
        {
            fn(node->value(Qt::ItemDataRole::DisplayRole).toString());
        }
    };

    if (node && tree)
    {
        emit_if_action("node.open", [&](QString node_name) {
            tree->project()->openElement(node_name);
        });
        emit_if_action("node.save", [&](QString node_name) {
            tree->project()->saveElement(node_name);
        });
        emit_if_action("node.delete", [&](QString node_name) {
            tree->project()->deleteElement(node_name);
        });
        emit_if_action("node.export", [&](QString node_name) {
            tree->project()->exportElement(node_name);
        });
    }
}
