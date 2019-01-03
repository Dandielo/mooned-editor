#pragma once
#include <settings/QSettings.h>

namespace editor
{

//! Class holding any arbitrary and persistent key-value entry for a project.
//! \note Loading and saving the settings file is done respectively on construction and destruction.
class QPersistentSettings : public QSettings
{
public:
    QPersistentSettings(QDir path) noexcept;
    ~QPersistentSettings() noexcept;

    //! Loads the settings file.
    void load() noexcept;

    //! Saves the settings file.
    void save() const noexcept;

private:
    QFileInfo _file_path;
};

} // namespace editor
