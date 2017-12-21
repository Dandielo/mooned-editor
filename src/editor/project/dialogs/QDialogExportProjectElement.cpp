#include "QDialogExportProjectElement.h"
#include "ui_export_element_dialog.h"

#include <QDialog>
#include <QFileDialog>

editor::QDialogExportProjectElement::QDialogExportProjectElement(QProjectElement* element)
    : QDialog{ nullptr }
    , _ui{ new Ui::ExportElementDialogUi{} }
    , _element{ element }
{
    _ui->setupUi(this);

    connect(_ui->buttonFindLocation, &QToolButton::clicked, this, &QDialogExportProjectElement::chooseOutputLocation);
    connect(_ui->buttonExport, &QPushButton::clicked, this, &QDialogExportProjectElement::exportToLocation);
    connect(_ui->buttonCancel, &QPushButton::clicked, this, &QDialogExportProjectElement::close);

    connect(_ui->editLocation, &QLineEdit::textChanged, [&](const QString& text) {
        if (text.isEmpty())
        {
            _ui->buttonExport->setEnabled(false);
        }
        else
        {
            _ui->buttonExport->setEnabled(true);
        }
    });


    // Default values
    _ui->buttonExport->setEnabled(false);
}

editor::QDialogExportProjectElement::~QDialogExportProjectElement()
{
    delete _ui;
}

void editor::QDialogExportProjectElement::exportToLocation()
{
    finished(_ui->editLocation->text());
    close();
}

void editor::QDialogExportProjectElement::chooseOutputLocation()
{
    QString file_name = QFileDialog::getSaveFileName(this, "Export file as...", QDir::homePath(), "Export file");

    // Update the edit line widget
    if (!file_name.isEmpty())
    {
        _ui->editLocation->setText(file_name);
    }
}

void editor::QDialogExportProjectElement::closeEvent(QCloseEvent* ev)
{
    deleteLater();
}

