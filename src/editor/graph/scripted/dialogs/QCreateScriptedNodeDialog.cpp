#include "QCreateScriptedNodeDialog.h"
#include "graph/scripted/QScriptedGraph.h"
#include "graph/scripted/dialogs/QScriptedGraphNodeTreeModel.h"

#include "ui_dialog_with_list.h"

editor::QCreateScriptedNodeDialog::QCreateScriptedNodeDialog(QScriptedGraph* graph, QVector<editor::script::Type> node_types, QWidget* parent /*= nullptr*/)
    : QDialog{ parent }
    , _ui{ nullptr }
    , _graph(graph)
{
    _ui = new Ui::DialogWithList{};
    _ui->setupUi(this);

    _model = new QScriptedGraphNodeTreeModel{ _graph->getScriptManager(), std::move(node_types) };
    _ui->treeView->setModel(_model);

    _ui->buttonCreate->setDisabled(true);

    connect(_ui->buttonCancel, &QPushButton::clicked, this, &QDialog::close);
    connect(_ui->buttonCreate, &QPushButton::clicked, this, &QCreateScriptedNodeDialog::createNodeAction);
    connect(_ui->treeView, &QTreeView::doubleClicked, this, &QCreateScriptedNodeDialog::createNodeAction);

    connect(_ui->treeView, &QTreeView::clicked, [&]() {
        _ui->buttonCreate->setDefault(false);
    });
}

editor::QCreateScriptedNodeDialog::~QCreateScriptedNodeDialog()
{
    delete _model;
}

void editor::QCreateScriptedNodeDialog::createNodeAction()
{
    auto index = _ui->treeView->currentIndex();
    auto classtype = _ui->treeView->model()->data(index, Qt::UserRole).toString();

    if (!classtype.isEmpty())
    {
        emit finished(classtype);
    }

    emit close();
}

void editor::QCreateScriptedNodeDialog::closeEvent(QCloseEvent* ev)
{
    deleteLater();
}

