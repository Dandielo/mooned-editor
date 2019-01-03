#include <project/tree/QTreeRoot.h>
#include <project/interfaces/QProject.h>

namespace editor
{

QProjectTreeRoot::QProjectTreeRoot(QPointer<QProject> project_element) noexcept
    : QProjectTreeNode{ nullptr, project_element->name() }
    , _project{ project_element }
{ }

auto QProjectTreeRoot::resource_uri() const noexcept -> QUrl
{
    return QUrl{ name() };
}

} // namespace editor
