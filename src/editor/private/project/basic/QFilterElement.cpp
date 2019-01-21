#include <project/basic/QFilterElement.h>

namespace editor
{

QFilterElement::QFilterElement(QProjectElement* parent, QFileInfo filter_file) noexcept
    : QProjectElement{ parent, filter_file.baseName() }
{
    settings().set_default("display_name", "Filter");
}

auto QFilterElement::value(Qt::ItemDataRole role) const noexcept -> QVariant
{
    if (role == Qt::ItemDataRole::DisplayRole)
    {
        return settings().get("display_name");
    }
    return { };
}

void QFilterElement::load() noexcept
{

}

void QFilterElement::save() const noexcept
{

}

} // namespace editor
