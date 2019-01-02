#pragma once
#include <QObject>
#include <QFileInfo>

namespace editor
{

//! The project class.
class QProject;

//! A simple base class for all entities a project may hold.
class QProjectElement : public QObject
{
    Q_OBJECT;

public:
    //! An object element is created with a valid project pointer.
    QProjectElement(QProjectElement* parent, QFileInfo element_file) noexcept;

    //! \returns The file associated for this project element.
    auto fileinfo() const noexcept -> const QFileInfo& { return _fileinfo; }

    //! \returns The element name.
    //! \note The element name is the base name of the file path.
    auto name() const noexcept -> QString;

    //! \returns The location where the file is located.
    auto location() const noexcept -> QDir;

    //! \returns A unique identifier for this element.
    auto identifier() const noexcept -> QString;

    //! \returns The project associated with this element.
    virtual auto project() noexcept -> QProject*;
    virtual auto project() const noexcept -> const QProject*;

    //! An empty implementation for loading project elements.
    virtual void load() noexcept { /* empty */ }

    //! An empty implementation for saving project elements.
    virtual void save() const noexcept { /* empty */ }

    //! \returns A value for the given item data role.
    virtual auto value(Qt::ItemDataRole role) const noexcept -> QVariant = 0;

private:
    //! The parent element
    QProjectElement* const _parent;

    //! The elements URI.
    QFileInfo _fileinfo;
};

}
