#pragma once
#include <QVector>

namespace editor
{
    class QProjectTreeNode
    {
    public:
        QProjectTreeNode(QProjectTreeNode* parent = nullptr);
        virtual ~QProjectTreeNode();

        uint index() const;
        uint childCount() const;

        QProjectTreeNode* parent() const;
        QProjectTreeNode* child(uint index) const;

        virtual QString toString() const;

    protected:
        QProjectTreeNode* _parent;
        QVector<QProjectTreeNode*> _childs;
    };
}
