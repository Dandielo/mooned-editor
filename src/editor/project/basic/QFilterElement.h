#pragma once
#include <project/interfaces/QProjectElement.h>
#include <settings/QSettings.h>

namespace editor
{

class QFilterElement : public QProjectElement
{
    Q_OBJECT;

public:
    QFilterElement(QProjectElement* parent, QFileInfo filter_file) noexcept;

    //! \returns The project settings object.
    auto settings() noexcept -> QSettings& { return _settings; }
    auto settings() const noexcept -> const QSettings& { return _settings; }

    //! \returns A value for the given item data role.
    auto value(Qt::ItemDataRole role) const noexcept -> QVariant override;

protected:
    //! Loads information about this filter from a file.
    void load() noexcept override;

    //! Saves information about this filter to a file.
    void save() const noexcept override;

private:
    QSettings _settings{ };
};

} // namespace editor
