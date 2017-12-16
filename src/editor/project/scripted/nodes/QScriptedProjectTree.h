#pragma once
#include "project/models/QProjectTree.h"

namespace editor
{
    class QScriptedProject;
    class QScriptedProjectTree : public editor::QProjectTree
    {
    public:
        QScriptedProjectTree(QScriptedProject* project);
        virtual ~QScriptedProjectTree() override;
    };
}
