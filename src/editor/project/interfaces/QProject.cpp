#include "QProject.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QFileInfo>
#include <cassert>

static const char* ProjectFileExtension = ".mprj";

editor::QProject::QProject(QString name, QDir location)
    : QObject{ nullptr }
    , _name{ name }
    , _location{ location }
{

}

editor::QProject::QProject()
    : QObject{ nullptr }
    , _name{ "" }
    , _location{ "" }
{
}

editor::QProject::~QProject()
{
}

bool editor::QProject::isValid() const
{
    return !_name.isEmpty();
}

bool editor::QProject::open(QFileInfo file_info)
{
    if (!file_info.exists())
    {
        return false;
    }

    // Read the project data
    QFile project_file{ file_info.canonicalFilePath() };
    QByteArray file_data = project_file.readAll();
    project_file.close();

    // Parse the given document
    QJsonDocument json_project = QJsonDocument::fromBinaryData(file_data);
    QJsonObject json_root = json_project.object();
    _name = json_root.value("name").toString();
    return true;
}

bool editor::QProject::save()
{
    if (!isValid())
    {
        return false;
    }

    // Create the JSon document
    QJsonObject json_root;
    json_root.insert("name", _name);

    QJsonDocument json_project(json_root);

    // Save the file
    QFileInfo file_info{ _location, _name + ProjectFileExtension };
    QFile project_file{ file_info.canonicalFilePath() };

    project_file.open(QFile::Text | QFile::WriteOnly);
    project_file.write(json_project.toJson());
    project_file.close();
    return true;
}

QString editor::QProject::name() const
{
    return _name;
}
