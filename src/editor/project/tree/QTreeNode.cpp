#include <project/tree/QTreeNode.h>

namespace editor
{

QProjectTreeNode::QProjectTreeNode(QProjectTreeNode* node_parent, QString node_name) noexcept
    : QObject{ node_parent }
    , _name{ std::move(node_name) }
{ }

auto QProjectTreeNode::parent_node() const noexcept -> QProjectTreeNode*
{
    return qobject_cast<QProjectTreeNode*>(parent());
}

auto QProjectTreeNode::child_nodes() const noexcept -> QList<QProjectTreeNode*>
{
    return findChildren<QProjectTreeNode*>();
}

auto QProjectTreeNode::resource_uri() const noexcept -> QUrl
{
    return parent_node()->resource_uri().resolved(name());
}

//////////////////////////////////////////////////////////////////////////

auto to_string(QProjectTreeNode::Type node_type) noexcept -> QString
{
#define CASE(value) case editor::QProjectTreeNode::Type::##value: return #value;
    switch (node_type)
    {
        CASE(Project);
        CASE(Filter);
        CASE(Graph);
        CASE(Custom);
    }
#undef CASE
}

auto from_string(const QString& node_type_name) noexcept -> QProjectTreeNode::Type
{
    QProjectTreeNode::Type result;
    if (node_type_name == "Project")
    {
        result = QProjectTreeNode::Type::Project;
    }
    else if (node_type_name == "Filter")
    {
        result = QProjectTreeNode::Type::Filter;
    }
    else if (node_type_name == "Graph")
    {
        result = QProjectTreeNode::Type::Graph;
    }
    else if (node_type_name == "Custom")
    {
        result = QProjectTreeNode::Type::Custom;
    }
    return result;
}

} // namespace editor
