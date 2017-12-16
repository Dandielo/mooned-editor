#include "QDialogNewProject.h"
#include "ui_dialog_with_file.h"

editor::QDialogNewProject::QDialogNewProject(QWidget* parent /* = nullptr */)
    : QDialog{ parent }
    , _ui{ new Ui::DialogWithFile }
{
    _ui->setupUi(this);
}

editor::QDialogNewProject::~QDialogNewProject()
{
    delete _ui;
}

