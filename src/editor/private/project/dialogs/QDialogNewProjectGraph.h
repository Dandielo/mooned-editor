#pragma once
#include <QDialog>

namespace Ui
{
    class DialogWithValue;
}

namespace editor
{
    class QProject;

    class QDialogNewProjectGraph : public QDialog
    {
        Q_OBJECT;

    public:
        QDialogNewProjectGraph(QProject* project, QWidget* parent = nullptr);
        virtual ~QDialogNewProjectGraph() override;

    signals:
        void finished(const QString& name);

    private slots:
        void checkAndFinish();

    protected:
        virtual void closeEvent(QCloseEvent* ev) override;

    private:
        Ui::DialogWithValue* _ui;
        QProject* _project;
    };
}
