#include <project/interfaces/QProjectElement.h>

#include <QDir>
#include <cassert>

namespace editor
{

QProjectElement::QProjectElement(QProjectElement* parent, QString element_name) noexcept
    : QProjectTreeNode{ parent, std::move(element_name) }
{
}

auto QProjectElement::project() noexcept -> QProject*
{
    return qobject_cast<QProjectElement*>(parent_node())->project();
}

auto QProjectElement::project() const noexcept -> const QProject*
{
    return qobject_cast<const QProjectElement*>(parent_node())->project();
}

bool QProjectElement::add_element(QProjectElement* element) noexcept
{
    // Try remove any other existing element
    bool element_found = contains_element(element->name());
    if (!element_found)
    {
        element->setParent(this); // Removes the element from the old parent and adds to this one.
    }

    // Set the element at the right place
    return element_found;
}

void QProjectElement::remove_element(const QString& identifier) noexcept
{
    QProjectElement* element = findChild<QProjectElement*>(identifier);
    if (element != nullptr)
    {
        element->setParent(nullptr); // Remove the element from this object.
        element->deleteLater();
    }
}

auto QProjectElement::get_element(const QString& identifier) noexcept -> QProjectElement*
{
    return findChild<QProjectElement*>(identifier);
}

auto QProjectElement::get_element(const QString& identifier) const noexcept -> const QProjectElement*
{
    return findChild<QProjectElement*>(identifier);
}

bool QProjectElement::contains_element(const QString& identifier) const noexcept
{
    return get_element(identifier) != nullptr;
}

auto QProjectElement::elements() noexcept -> QList<QProjectElement*>
{
    return findChildren<QProjectElement*>(QString{}, Qt::FindDirectChildrenOnly);
}

auto QProjectElement::elements() const noexcept -> QList<const QProjectElement*>
{
    return findChildren<const QProjectElement*>(QString{}, Qt::FindDirectChildrenOnly);
}

} // namespace editor
