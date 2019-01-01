#pragma once
#include <QVariant>

#include <vector>
#include <memory>

namespace editor
{

//! A tree node for a project TreeView model.
class ProjectTreeNode
{
public:
    //! Create a tree node with a parent.
    ProjectTreeNode(ProjectTreeNode* parent_node) noexcept;
    virtual ~ProjectTreeNode() noexcept = default;

    //! \returns The index of the node in it's scope.
    auto index() const noexcept ->size_t;

    //! \returns The parent tree node.
    auto parent() noexcept -> ProjectTreeNode* { return _parent; }
    auto parent() const noexcept -> const ProjectTreeNode* { return _parent; }

    //! \returns The number of child nodes.
    auto child_count() const noexcept -> size_t { return _childs.size(); }

    //! \returns The child node at the given index.
    auto child(size_t child_index) noexcept -> ProjectTreeNode*;
    auto child(size_t child_index) const noexcept -> const ProjectTreeNode*;

    //! Adds a new child node.
    void add(std::unique_ptr<ProjectTreeNode> child_node) noexcept;

    //! Removes the given child node.
    void remove(size_t child_index) noexcept;

    //! \returns A value for the given item data role.
    virtual auto value(Qt::ItemDataRole role) const noexcept -> QVariant = 0;

private:
    //! Holds the parent node.
    ProjectTreeNode* _parent;

    //! Holds a vector of children.
    std::vector<std::unique_ptr<ProjectTreeNode>> _childs{ };
};

} // namespace editor
