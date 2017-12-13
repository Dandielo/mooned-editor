#include "QProjectTreeNode.h"

editor::QProjectTreeNode::QProjectTreeNode(QProjectTreeNode* parent /*= nullptr*/)
    : _parent{ parent }
    , _childs{ }
{
}

editor::QProjectTreeNode::~QProjectTreeNode()
{
    for (auto&& child : _childs)
    {
        delete child;
    }
}

uint editor::QProjectTreeNode::index() const
{
    if (_parent)
    {
        return _parent->_childs.indexOf(const_cast<QProjectTreeNode*>(this));
    }
    return 0;
}

uint editor::QProjectTreeNode::childCount() const
{
    return _childs.count();
}

QString editor::QProjectTreeNode::toString() const
{
    return "{node}";
}

editor::QProjectTreeNode* editor::QProjectTreeNode::parent() const
{
    return _parent;
}

editor::QProjectTreeNode* editor::QProjectTreeNode::child(uint index) const
{
    return _childs.at(index);
}