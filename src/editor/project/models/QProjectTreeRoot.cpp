#include <project/models/QProjectTreeRoot.h>
#include <project/interfaces/QProject.h>

namespace editor
{

ProjectTreeRoot::ProjectTreeRoot(QProject* prj) noexcept
    : ProjectTreeNode{ nullptr }
    , _project{ prj }
{ }

auto ProjectTreeRoot::value(Qt::ItemDataRole role) const noexcept -> QVariant
{
    if (role == Qt::ItemDataRole::DisplayRole)
    {
        return _project->name();
    }
    return { };
}

} // namespace editor
