#include <project/basic/QProjectSettings.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>

namespace editor
{

QProjectSettings::QProjectSettings(QDir location) noexcept
    : _file_path{ std::move(location), "settings.json" }
{
    load();
}

QProjectSettings::~QProjectSettings() noexcept
{
    save();
}

void QProjectSettings::load() noexcept
{
    // Try to open the file, create if not existing
    QFile file{ _file_path.absoluteFilePath() };
    file.open(QFile::OpenModeFlag::ReadWrite);

    // Try to parse the json document #todo dont ignore on errors
    QJsonDocument json_document{ QJsonDocument::fromJson(file.readAll()) };
    if (json_document.isObject())
    {
        // Clear the current map of settings
        _entries.clear();

        // Read the json dictionary
        QJsonObject json_settings = json_document.object();
        for (const auto& key : json_settings.keys())
        {
            _entries.insert(key, json_settings.value(key).toVariant());
        }
    }
}

void QProjectSettings::save() const noexcept
{
    // Try to open the file, create if not existing
    QFile file{ _file_path.absoluteFilePath() };
    file.open(QFile::OpenModeFlag::WriteOnly);

    // Iterate over all keys
    QJsonObject root;
    for (const auto& key : _entries.keys())
    {
        auto json_val = QJsonValue::fromVariant(_entries.value(key));
        assert(json_val.isString() || json_val.isBool() || json_val.isDouble());
        root.insert(key, json_val);
    }

    // Write the whole document at once
    QJsonDocument json_document{ root };
    file.write(json_document.toJson());
    file.close();
}

void QProjectSettings::set(QString key, QVariant value) noexcept
{
    assert(value.isValid());

    if (_entries.contains(key))
    {
        _entries.remove(key);
    }

    _entries.insert(key, value);
}

void QProjectSettings::set_default(QString key, QVariant value) noexcept
{
    assert(value.isValid());

    if (!_entries.contains(key))
    {
        _entries.insert(key, value);
    }
}

auto QProjectSettings::get(QString key) const noexcept -> const QVariant&
{
    return *_entries.find(key);
}

} // namespace editor
