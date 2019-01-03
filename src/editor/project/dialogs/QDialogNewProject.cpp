#include "QDialogNewProject.h"
#include "ui_project_create.h"

#include <QFileDialog>

editor::QProjectTypesModel::QProjectTypesModel(const std::map<std::string, project::FactoryFunction>& project_types)
    : QAbstractItemModel{ }
    , _project_types{ }
{
    for (const auto& entry : project_types)
    {
        _project_types.append(QString::fromStdString(entry.first));
    }
}

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


namespace editor
{

QDialogNewProject::QDialogNewProject(QEditorMainWindow* main_window)
    : QDialog{ main_window }
    , _ui{ std::make_unique<Ui::DialogCreateProject>() }
    , _model{ main_window->project_factories() }
{
    _ui->setupUi(this);

    // Setup all default dialog connections
    connect(_ui->projectDialogCancel, &QPushButton::clicked, this, &QDialogNewProject::check_and_finish);
    connect(_ui->projectDialogCreate, &QPushButton::clicked, this, &QDialogNewProject::check_and_finish);
    connect(this, &QDialogNewProject::finished, main_window, &QEditorMainWindow::open_project);

    // Set the project model for the list of project types.
    _ui->projectTypeValue->setModel(&_model);

    // Prepare the UI
    _ui->projectDialogCreate->setEnabled(false);

    _ui->projectNameValue->setPlaceholderText("A project name. (min 3 characters)");

    _ui->projectLocationValue->setReadOnly(true);
    _ui->projectLocationValue->setPlaceholderText("The project location value.");
    _ui->projectLocationValue->setText(main_window->settings().get("projects.location").toString());

    // Connect actions
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

void editor::QDialogNewProject::check_and_finish() noexcept
{
    if (sender() == _ui->projectDialogCreate)
    {
        editor::project::FactoryData factory_data;
        factory_data.class_name = _ui->projectTypeValue->currentData(Qt::UserRole).toString().toStdString();
        factory_data.name = _ui->projectNameValue->text().toStdString();
        factory_data.file = QDir::cleanPath(_ui->projectLocationValue->text() + "/" + QString::fromStdString(factory_data.name) + ".mprj");

        emit finished(factory_data);
    }
    else
    {
        emit canceled();
    }

    close();
}

} // namespace editor
