#pragma once
#include <project/models/QProjectTreeNode.h>

namespace editor
{

//! The project base class.
class QProject;

//! The root node of a project tree.
//! \note This tree root is owned by the represented QProject object.
class ProjectTreeRoot : public ProjectTreeNode
{
public:
    //! Creates a new tree root from a project object.
    ProjectTreeRoot(QProject* prj) noexcept;

    //! \returns The project which is represented by this tree root.
    auto project() noexcept -> QProject* { return _project; }
    auto project() const noexcept -> const QProject* { return _project; }

    //! \returns a value for the given item data role.
    auto value(Qt::ItemDataRole role) const noexcept -> QVariant override;

private:
    QProject* _project;
};

} // namespace editor
