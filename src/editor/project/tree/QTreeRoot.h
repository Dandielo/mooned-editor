#pragma once
#include <project/tree/QTreeNode.h>
#include <QFileInfo>
#include <QPointer>

namespace editor
{

//! The project class.
class QProject;

//! Root node.
//! \note The root node of a project tree.
class QProjectTreeRoot : public QProjectTreeNode
{
    Q_OBJECT;
public:
    //! Creates a new project tree from a project it represents.
    QProjectTreeRoot(QPointer<QProject> project_element) noexcept;

    //! \returns The tree node URI identifier.
    auto resource_uri() const noexcept -> QUrl override;

    //! \returns The tree node type.
    auto type() const noexcept -> Type override { return QProjectTreeNode::Type::Project; }

private:
    //! The project represented by this tree.
    QPointer<QProject> _project;
};

} // namespace editor
