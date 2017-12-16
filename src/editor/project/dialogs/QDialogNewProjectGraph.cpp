#include "QDialogNewProjectGraph.h"
#include "ui_dialog_with_value.h"

#include "project/interfaces/QProject.h"

#include <cassert>

editor::QDialogNewProjectGraph::QDialogNewProjectGraph(editor::QProject* project, QWidget* parent /* = nullptr */)
    : QDialog{ parent }
    , _ui{ new Ui::DialogWithValue }
    , _project{ project }
{
    assert(_project != nullptr);

    _ui->setupUi(this);

    // Connect actions
    connect(_ui->buttonCancel, &QPushButton::clicked, this, &QDialog::close);
    connect(_ui->buttonCreate, &QPushButton::clicked, this, &QDialogNewProjectGraph::checkAndFinish);

    _ui->buttonCreate->setEnabled(false);

    // Connect checks
    connect(_ui->editValue, &QLineEdit::textChanged, [&](const QString& text) {
        _ui->buttonCreate->setEnabled(!text.isEmpty());
    });
}

editor::QDialogNewProjectGraph::~QDialogNewProjectGraph()
{
    delete _ui;
}

void editor::QDialogNewProjectGraph::checkAndFinish()
{
    if (!_project->hasElement(_ui->editValue->text()))
    {
        emit finished(_ui->editValue->text());
    }
    else
    {
        emit rejected();
    }

    close();
}

void editor::QDialogNewProjectGraph::closeEvent(QCloseEvent*)
{
    deleteLater();
}

