#include <settings/QSettings.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>

namespace editor
{

void QSettings::load(const QJsonObject& from) noexcept
{
    // Read the json dictionary
    for (const auto& key : from.keys())
    {
        _entries.insert(key, from.value(key).toVariant());
    }
}

void QSettings::save(QJsonObject& to) const noexcept
{
    // Iterate over all keys
    for (const auto& key : _entries.keys())
    {
        auto json_val = QJsonValue::fromVariant(_entries.value(key));
        assert(json_val.isString() || json_val.isBool() || json_val.isDouble());
        to.insert(key, json_val);
    }
}

void QSettings::set(QString key, QVariant value) noexcept
{
    assert(value.isValid());

    if (_entries.contains(key))
    {
        _entries.remove(key);
    }

    _entries.insert(key, value);
}

void QSettings::set_default(QString key, QVariant value) noexcept
{
    assert(value.isValid());

    if (!_entries.contains(key))
    {
        _entries.insert(key, value);
    }
}

auto QSettings::get(QString key) const noexcept -> QVariant
{
    assert(_entries.contains(key));
    return *_entries.find(key);
}

auto QSettings::get(QString key, QVariant default_value) const noexcept -> QVariant
{
    auto entry_it = _entries.find(key);
    if (entry_it != _entries.end())
    {
        default_value = *entry_it;
    }
    return default_value;
}

} // namespace editor
