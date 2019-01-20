#pragma once
#include <QObject>
#include <QVariant>
#include <QFileInfo>

#include <project/interfaces/QProjectTreeNode.h>

namespace editor
{

//! The project class.
class QProject;

//! A simple base class for all entities a project may hold.
class QProjectElement : public QProjectTreeNode
{
    Q_OBJECT;

public:
    //! An object element is created with a valid project pointer.
    QProjectElement(QProjectElement* parent, QString element_name) noexcept;

    //! \returns The project associated with this element.
    virtual auto project() noexcept -> QProject*;
    virtual auto project() const noexcept -> const QProject*;

    //! An empty implementation for loading project elements.
    virtual void load() noexcept { /* empty */ }

    //! An empty implementation for saving project elements.
    virtual void save() const noexcept { /* empty */ }

public:
    //! Adds the given element to the project.
    //! \param[in] element The element to be added to the project.
    //! \returns true If the element was added to the project.
    bool add_element(QProjectElement* element) noexcept;

    //! Tries to remove the given element from the project.
    //! \param[in] identifier A string value representing a unique id the the project.
    //! \note If the element is not part of the project, nothing will happen.
    void remove_element(const QString& identifier) noexcept;

    //! Searches for an element of the given identifier and returns a pointer to it.
    //! \param[in] identifier A string value representing a unique id the the project.
    //! \returns A pointer to the requested element or nullptr if nothing was found.
    auto get_element(const QString& identifier) noexcept -> QProjectElement*;
    auto get_element(const QString& identifier) const noexcept -> const QProjectElement*;

    //! Checks if the project contains an element with the given identifier.
    //! \param[in] identifier A string value representing a unique id the the project.
    bool contains_element(const QString& identifier) const noexcept;

    //! \returns A list of all elements in the project.
    auto elements() noexcept -> QList<QProjectElement*>;
    auto elements() const noexcept -> QList<const QProjectElement*>;
};

} // namespace editor
