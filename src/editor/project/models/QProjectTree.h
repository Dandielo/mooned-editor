#pragma once
#include "QProjectTreeNode.h"

namespace editor
{
    class QProject;

    class QProjectTree : public QProjectTreeNode
    {
    public:
        QProjectTree(QProject* project);
        virtual ~QProjectTree() override;

        QProject* project() const;

        virtual QString toString() const override;

    private:
        QProject* _project;
    };
}
