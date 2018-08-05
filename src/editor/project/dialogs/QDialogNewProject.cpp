#include "QDialogNewProject.h"
#include "ui_project_create.h"

#include <QFileDialog>

editor::QProjectTypesModel::QProjectTypesModel(const QVector<QString>& project_types)
    : QAbstractItemModel{ }
    , _project_types{ project_types }
{ }

editor::QProjectTypesModel::~QProjectTypesModel()
{

}

int editor::QProjectTypesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
    {
        return 0;
    }
    if (parent.isValid())
    {
        return 0;
    }

    return _project_types.size();
}

int editor::QProjectTypesModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex editor::QProjectTypesModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex()*/) const
{
    if (!hasIndex(row, column, parent))
    {
        return {};
    }

    return createIndex(row, column);
}

QModelIndex editor::QProjectTypesModel::parent(const QModelIndex &child) const
{
    return { };
}

QVariant editor::QProjectTypesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }

    if (role == Qt::UserRole)
    {
        return { _project_types.at(index.row()) };
    }

    if (role != Qt::DisplayRole)
    {
        return { };
    }

    return { _project_types.at(index.row()) };
}

QVariant editor::QProjectTypesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return "Project type";

    return {};
}

//////////////////////////////////////////////////////////////////////////

editor::QDialogNewProject::QDialogNewProject(QEditorMainWindow* main_window, QWidget* parent /*= nullptr*/)
    : QDialog{ parent }
    , _ui{ new Ui::DialogCreateProject() }
    , _projects{ main_window->getProjectTypes() }
    , _model{ _projects.keys().toVector() }
{
    _ui->setupUi(this);
    _ui->projectTypeValue->setModel(&_model);

    // Prepare the UI
    _ui->projectDialogCreate->setEnabled(false);

    _ui->projectNameValue->setPlaceholderText("A project name. (min 3 characters)");

    _ui->projectLocationValue->setReadOnly(true);
    _ui->projectLocationValue->setPlaceholderText("The project location value.");
    _ui->projectLocationValue->setText(main_window->defalutProjectLocation());

    // Connect actions
    connect(_ui->projectDialogCancel, &QPushButton::clicked, this, &QDialog::close);
    connect(_ui->projectDialogCreate, &QPushButton::clicked, this, &QDialogNewProject::checkAndFinish);
    connect(_ui->projectLocationChoose, &QToolButton::clicked, this, [this, main_window]()
        {
            QString project_location = QFileDialog::getExistingDirectory(this, "Project location...", _ui->projectLocationValue->text());
            _ui->projectLocationValue->setText(project_location);
        });

    // Connect checks
    connect(_ui->projectNameValue, &QLineEdit::textChanged, [&](const QString& text)
        {
            _ui->projectDialogCreate->setEnabled(text.length() >= 3);
            if (text.length() >= 0)
            {
                _ui->projectDialogCreate->setDefault(true);
            }
        });
}

editor::QDialogNewProject::~QDialogNewProject()
{
    delete _ui;
}

void editor::QDialogNewProject::checkAndFinish()
{
    QString selected_type = _ui->projectTypeValue->currentData(Qt::UserRole).toString();
    QString project_name = _ui->projectNameValue->text();
    QString project_location = _ui->projectLocationValue->text();

    QString full_path = QDir::cleanPath(project_location + "/" + project_name + ".mprj");

    auto selected_factory = _projects.find(selected_type);
    assert(selected_factory != _projects.end());

    QProject* project = selected_factory->factory(selected_type, project_name, full_path, selected_factory->userdata);

    emit finished(project);
    close();
}

