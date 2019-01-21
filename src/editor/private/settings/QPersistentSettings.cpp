#include <meditor/settings/QPersistentSettings.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>

namespace editor
{

QPersistentSettings::QPersistentSettings(QDir location) noexcept
    : QSettings{ }
    , _file_path{ std::move(location), "settings.json" }
{
    [[maybe_unused]] bool location_exists = _file_path.absoluteDir().mkpath(".");
    assert(location_exists);

    load();
}

QPersistentSettings::~QPersistentSettings() noexcept
{
    save();
}

void QPersistentSettings::load() noexcept
{
    // Try to open the file, create if not existing
    QFile file{ _file_path.absoluteFilePath() };
    file.open(QFile::OpenModeFlag::ReadWrite);

    // Try to parse the json document #todo don't ignore on errors
    QJsonDocument json_document{ QJsonDocument::fromJson(file.readAll()) };
    if (json_document.isObject())
    {
        QSettings::load(json_document.object());
    }
}

void QPersistentSettings::save() const noexcept
{
    // Try to open the file, create if not existing
    QFile file{ _file_path.absoluteFilePath() };
    file.open(QFile::OpenModeFlag::WriteOnly);

    // Iterate over all keys
    QJsonObject root;
    QSettings::save(root);

    // Write the whole document at once
    QJsonDocument json_document{ root };
    file.write(json_document.toJson());
    file.close();
}

} // namespace editor
