#pragma once
#include <QObject>
#include <QUrl>

namespace editor
{

//! Project tree node.
//! \note A project is just a tree of nodes, where each node may be the owner of a project element.
class QProjectTreeNode : public QObject
{
    Q_OBJECT;

public:
    //! Enum class of possible node types.
    enum class Type
    {
        Project, Filter, Graph, Custom
    };

public:
    //! Creates a new node with the given name.
    QProjectTreeNode(QProjectTreeNode* par_node, QString node_name) noexcept;
    virtual ~QProjectTreeNode() noexcept = default;

    //! \returns The tree node name.
    auto name() const noexcept -> const QString& { return _name; }

    //! \returns The parent node or nullptr for the root node.
    auto parent_node() const noexcept -> QProjectTreeNode*;

    //! \returns All child nodes.
    auto child_nodes() const noexcept -> QList<QProjectTreeNode*>;

    //! \returns The tree node URI identifier.
    virtual auto resource_uri() const noexcept -> QUrl;

    //! \returns The tree node type.
    virtual auto type() const noexcept -> Type = 0;

private:
    //! The name of the node.
    //! \note This value is also used to during the serialization or export process.
    QString _name;
};

//! \returns The string representation of the given tree node.
auto to_string(QProjectTreeNode::Type node_type) noexcept -> QString;

//! \returns The tree node type represented by the string.
auto from_string(const QString& node_type_name) noexcept -> QProjectTreeNode::Type;

} // namespace editor
