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

QProject::QProject(QFileInfo project_file) noexcept
    : QProjectElement{ nullptr, std::move(project_file) }
    , _settings{ location() }
{
    _settings.set_default("project.display_name", name());
    _settings.set_default("export.location", QDir::cleanPath(location().absoluteFilePath("exported")));
}

void QProject::load() noexcept
{
    // Check if the values are valid #todo Is it good to store these vales so we can later create a 'new' project when this function fails?
    if (!fileinfo().exists())
    {
        return;
    }

    QFile project_file{ fileinfo().absoluteFilePath() };
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
    QFile project_file{ fileinfo().absoluteFilePath() };
    project_file.open(QFile::OpenModeFlag::WriteOnly);

    if (!project_file.isOpen())
    {
        return;
    }

    // Create the JSon root object
    QJsonObject json_root;
    onSave(json_root);

    json_root.insert("name", name());
    json_root.insert("class", class_name());
    json_root.insert("version", version().toString());

    // Create the JSon document.
    QJsonDocument json_project(json_root);

    // Write the document to file
    project_file.write(json_project.toJson());
    project_file.close();
    return;
}

bool QProject::add_element(QProjectElementPtr element) noexcept
{
    assert(element->parent() == this); // We want an element to be without a parent when it's added!

    // Set the new parent object #todo maybe create objects with some kind of factory function? (again)
    //element->setParent(this);

    // Try remove any other existing element
    auto element_identifier = element->name();
    bool element_found = contains_element(element_identifier);
    if (!element_found)
    {
        _elements.insert(std::move(element_identifier), std::move(element));
    }

    // Set the element at the right place
    return element_found;
}

void QProject::remove_element(const QString& identifier) noexcept
{
    if (_elements.contains(identifier))
    {
        (*_elements.find(identifier))->deleteLater();
        _elements.remove(identifier);
    }
}

auto QProject::get_element(const QString& identifier) noexcept -> QProjectElementPtr
{
    auto element_it = _elements.find(identifier);
    if (element_it != _elements.end())
    {
        return *element_it;
    }
    return nullptr;
}

auto QProject::get_element(const QString& identifier) const noexcept -> const QProjectElementPtr
{
    auto element_it = _elements.find(identifier);
    if (element_it != _elements.end())
    {
        return *element_it;
    }
    return nullptr;
}

bool QProject::contains_element(const QString& identifier) const noexcept
{
    return _elements.contains(identifier);
}

} // namespace editor
