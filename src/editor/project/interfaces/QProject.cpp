#include "QProject.h"
#include "QProjectElement.h"
#include "windows/QEditorMainWindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QFileInfo>
#include <QDebug>
#include <cassert>

static const char* ProjectFileExtension = ".mprj";

editor::QProject::QProject()
    : QObject{ nullptr }
    , _name{ "" }
    , _filename{ "" }
    , _location{ "" }
    , _elements{}
{
}

editor::QProject::~QProject()
{
    for (auto* element : _elements)
    {
        delete element;
    }
    _elements.clear();
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

    // Set the file name and location values
    _filename = file_info.fileName();
    _location = file_info.absoluteDir();

    // Read the project data
    QJsonObject json_root = loadProjectFile(file_info).object();
    onLoad(json_root);

    _name = json_root.value("name").toString();
    _type = json_root.value("type").toString();
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
    onSave(json_root);

    json_root.insert("name", _name);
    json_root.insert("version", "alpha");
    json_root.insert("type", _type);

    QJsonDocument json_project(json_root);

    // Save the file
    QFileInfo file_info{ _location, _filename };
    QFile project_file{ file_info.absoluteFilePath() };

    project_file.open(QFile::Text | QFile::WriteOnly);
    project_file.write(json_project.toJson());
    project_file.close();
    return true;
}

bool editor::QProject::hasElement(QString name) const
{
    return _elements.count(name) > 0;
}

bool editor::QProject::hasElement(QProjectElement* element)
{
    return hasElement(element->name());
}

void editor::QProject::addElement(QProjectElement* element)
{
    assert(!hasElement(element));
    _elements.insert(element->name(), element);
}

void editor::QProject::removeElement(QProjectElement* element)
{
    assert(hasElement(element));
    _elements.remove(element->name());
}

QString editor::QProject::name() const
{
    return _name;
}

QString editor::QProject::type() const
{
    return _type;
}

QString editor::QProject::filename() const
{
    return _filename;
}

QDir editor::QProject::location() const
{
    return _location;
}

// Free function
QJsonDocument editor::loadProjectFile(QFileInfo path)
{
    QFile project_file{ path.absoluteFilePath() };
    project_file.open(QFile::Text | QFile::ReadOnly);
    QByteArray file_data = project_file.readAll();
    project_file.close();

    // Parse the given document
    return QJsonDocument::fromJson(file_data);
}
