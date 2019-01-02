#pragma once
#include <QFileInfo>
#include <QVariant>
#include <QMap>

namespace editor
{

//! Class holding any arbitrary and persistent key-value entry for a project.
//! \note Loading and saving the settings file is done respectively on construction and destruction.
class QProjectSettings final
{
public:
    QProjectSettings(QDir path) noexcept;
    ~QProjectSettings() noexcept;

    //! Loads the settings file.
    void load() noexcept;

    //! Saves the settings file.
    void save() const noexcept;

    //! Sets the value for the given key.
    //! \param key The setting key value
    //! \param value A value of type 'int', 'float', 'string' or 'bool'.
    void set(QString key, QVariant value) noexcept;

    //! Sets the value for the given key if not existing.
    //! \param key The setting key value
    //! \param value A value of type 'int', 'float', 'string' or 'bool'.
    void set_default(QString key, QVariant value) noexcept;

    //! \returns the value for the given key.
    auto get(QString key) const noexcept -> const QVariant&;

    //! \returns the value for the given key or the default value.
    auto get(QString key, QVariant default_value) const noexcept -> const QVariant&;

private:
    QFileInfo _file_path;
    QMap<QString, QVariant> _entries;
};

} // namespace editor
