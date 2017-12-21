#pragma once
#include <QDialog>

namespace Ui
{
    class ExportElementDialogUi;
}

namespace editor
{
    class QProjectElement;

    class QDialogExportProjectElement : public QDialog
    {
        Q_OBJECT;

    public:
        QDialogExportProjectElement(QProjectElement* element);
        virtual ~QDialogExportProjectElement() override;

    signals:
        void finished(const QString& path);

    protected slots:
        void exportToLocation();
        void chooseOutputLocation();

    protected:
        virtual void closeEvent(QCloseEvent* ev) override;

    private:
        Ui::ExportElementDialogUi* _ui;
        QProjectElement* _element;
    };
}