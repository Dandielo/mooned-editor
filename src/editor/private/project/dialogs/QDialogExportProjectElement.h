#pragma once
#include <QDialog>
#include "scripts/CScriptManager.h"

namespace Ui
{
    class ExportElementDialogUi;

    using Ptr = std::unique_ptr<ExportElementDialogUi>;
}

namespace editor
{
    class QProjectElement;

    class QDialogExportProjectElement : public QDialog
    {
        Q_OBJECT;

    public:
        QDialogExportProjectElement(Scripts::CScriptManager* script_Manager, const QVector<QString>& exporters, const QString& location, QProjectElement* element) noexcept;
        virtual ~QDialogExportProjectElement() noexcept override = default;

    signals:
        void finished(const QString& exporter, const QString& path);

    protected slots:
        void exportToLocation();
        void chooseOutputLocation();

    protected:
        virtual void closeEvent(QCloseEvent* ev) override;

    private:
        // Holds the dialog UI class.
        const Ui::Ptr _ui;

        // Holds the script manager.
        Scripts::CScriptManager* _script_manager;

        // Holds reference to all available exporters.
        const QVector<QString>& _exporters;

        // Holds the project element this dialog was created for.
        QProjectElement* _element;
    };
}