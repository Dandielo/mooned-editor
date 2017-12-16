#pragma once
#include <QVector>

namespace editor
{
    class QProjectTreeNode
    {
    public:
        QProjectTreeNode(QProjectTreeNode* parent);
        virtual ~QProjectTreeNode();

        uint index() const;
        uint childCount() const;

        QProjectTreeNode* parent() const;
        QProjectTreeNode* child(uint index) const;

        void add(QProjectTreeNode* node);
        void remove(QProjectTreeNode* node);

        virtual QString toString() const;

    protected:
        QProjectTreeNode* _parent;
        QVector<QProjectTreeNode*> _childs;
    };
}
