#include "QScriptedProjectTree.h"
#include "project/scripted/QScriptedProject.h"

editor::QScriptedProjectTree::QScriptedProjectTree(QScriptedProject* project)
    : QProjectTree{ project }
{
}

editor::QScriptedProjectTree::~QScriptedProjectTree()
{
}
