#include "QProject.h"
#include "QProjectElement.h"
#include "windows/QEditorMainWindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QPointer>
#include <QFileInfo>
#include <QDebug>
#include <cassert>

namespace editor
{

QProject::QProject(const QFileInfo& project_file) noexcept
    : QProjectElement{ nullptr, project_file.baseName() }
    , _location{ project_file.absoluteDir() }
    , _settings{ _location }
{
    _settings.set_default("project.display_name", name());
    _settings.set_default("export.location", QDir::cleanPath(_location.absoluteFilePath("exported")));
}

void QProject::load() noexcept
{
    QFileInfo fileinfo{ _location, name() + ".mprj" };

    // Check if the values are valid #todo Is it good to store these vales so we can later create a 'new' project when this function fails?
    if (!fileinfo.exists())
    {
        return;
    }

    QFile project_file{ fileinfo.absoluteFilePath() };
    project_file.open(QFile::OpenModeFlag::ReadWrite);

    auto project_document = QJsonDocument::fromJson(project_file.readAll());
    if (!project_document.isObject())
    {
        return;
    }

    // Assume we load always the latest version
    QVersionNumber loaded_version = version();

    // Load the actual project version from file.
    auto json_root = project_document.object();
    if (json_root.contains("version"))
    {
        loaded_version = QVersionNumber::fromString(json_root.value("version").toString());
    }

    // Continue with the loading
    onLoad(json_root, loaded_version);
    return;
}

void QProject::save() const noexcept
{
    QFileInfo fileinfo{ _location, name() + ".mprj" };

    QFile project_file{ fileinfo.absoluteFilePath() };
    project_file.open(QFile::OpenModeFlag::WriteOnly);

    if (!project_file.isOpen())
    {
        return;
    }

    // Create the JSon root object
    QJsonObject json_root;
    onSave(json_root);

    json_root.insert("name", name());
    json_root.insert("class", className());
    json_root.insert("version", version().toString());

    // Create the JSon document.
    QJsonDocument json_project(json_root);

    // Write the document to file
    project_file.write(json_project.toJson());
    project_file.close();
    return;
}

} // namespace editor
