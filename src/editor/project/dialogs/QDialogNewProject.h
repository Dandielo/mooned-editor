#pragma once
#include <QDialog>

namespace Ui
{
    class DialogWithFile;
}

namespace editor
{
    class QDialogNewProject : public QDialog
    {
        Q_OBJECT;

    public:
        QDialogNewProject(QWidget* parent = nullptr);
        virtual ~QDialogNewProject() override;

    private:
        Ui::DialogWithFile* _ui;
    };
}
