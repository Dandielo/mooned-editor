#include "QDialogExportProjectElement.h"
#include "ui_export_element_dialog.h"

#include "project/scripted/QScriptedProjectExporter.h"
#include "project/scripted/elements/QScriptedElementGraph.h"

#include "graph/scripted/QScriptedGraph.h"

#include <QDialog>
#include <QFileDialog>

namespace editor
{

QDialogExportProjectElement::QDialogExportProjectElement(Scripts::CScriptManager* script_manager, const QVector<QString>& exporters, const QString& location, QProjectElement* element) noexcept
    : QDialog{ nullptr }
    , _ui{ std::make_unique<Ui::ExportElementDialogUi>() }
    , _script_manager{ script_manager }
    , _exporters{ exporters }
    , _element{ element }
{
    _ui->setupUi(this);

    connect(_ui->buttonFindLocation, &QToolButton::clicked, this, &QDialogExportProjectElement::chooseOutputLocation);
    connect(_ui->buttonExport, &QPushButton::clicked, this, &QDialogExportProjectElement::exportToLocation);
    connect(_ui->buttonCancel, &QPushButton::clicked, this, &QDialogExportProjectElement::close);

    connect(_ui->editLocation, &QLineEdit::textChanged, [&](const QString& text)
        {
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
    _ui->editLocation->setText(location);
    _ui->buttonExport->setEnabled(false);

    // Set the list of exporters
    for (const auto& exporter : exporters)
    {
        _ui->exporterList->addItem(exporter);
    }

    if (!exporters.empty())
    {
        _ui->exporterList->setCurrentItem(_ui->exporterList->item(0));
    }
}

void QDialogExportProjectElement::exportToLocation()
{
    auto exporter = _ui->exporterList->currentItem()->text();

    {
        // Create a new exporter object.
        auto* exporter_object = _script_manager->CreateObject<QScriptedProjectExporter, 1>(exporter.toStdString());
        exporter_object->export_project_element(_element);

        // Release the object afterwards
        delete exporter_object;
    }

    finished(exporter, _ui->editLocation->text());
    close();
}

void QDialogExportProjectElement::chooseOutputLocation()
{
    QString file_name = QFileDialog::getSaveFileName(this, "Export file as...", QDir::homePath(), "*.lua");

    // Update the edit line widget
    if (!file_name.isEmpty())
    {
        _ui->editLocation->setText(file_name);
        _ui->buttonExport->setEnabled(true);
    }
}

void QDialogExportProjectElement::closeEvent(QCloseEvent* ev)
{
    deleteLater();
}

} // namespace editor
