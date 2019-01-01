#include <project/models/QProjectTreeNode.h>

#include <algorithm>
#include <cassert>

namespace editor
{

ProjectTreeNode::ProjectTreeNode(ProjectTreeNode* parent) noexcept
    : _parent{ parent }
{ }

auto editor::ProjectTreeNode::index() const noexcept -> size_t
{
    size_t result = 0;
    if (_parent)
    {
        const auto& parent_childs = _parent->_childs;

        // Get the position in the parent vector
        result = std::distance(parent_childs.begin(), std::find_if(parent_childs.begin(), parent_childs.end(), [this](const auto& el)
            {
                // Search for an element with the same pointer value.
                return el.get() == this;
            }));
    }
    return result;
}

auto ProjectTreeNode::child(size_t child_index) noexcept -> ProjectTreeNode*
{
    assert(_childs.size() > child_index);

    ProjectTreeNode* result = nullptr;
    if (_childs.size() > child_index)
    {
        result = _childs.at(child_index).get();
    }
    return result;
}

auto ProjectTreeNode::child(size_t child_index) const noexcept -> const ProjectTreeNode*
{
    assert(_childs.size() > child_index);

    ProjectTreeNode* result = nullptr;
    if (_childs.size() > child_index)
    {
        result = _childs.at(child_index).get();
    }
    return result;
}

void ProjectTreeNode::add(std::unique_ptr<ProjectTreeNode> child_node) noexcept
{
    _childs.emplace_back(std::move(child_node));
}

void ProjectTreeNode::remove(size_t child_index) noexcept
{
    assert(_childs.size() > child_index);
    _childs.erase(std::next(_childs.begin(), child_index));
}

} // namespace editor
