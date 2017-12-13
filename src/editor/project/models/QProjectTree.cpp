#include "QProjectTree.h"
#include "project/interfaces/QProject.h"

editor::QProjectTree::QProjectTree(QProject* project)
    : QProjectTreeNode{ nullptr }
    , _project{ project }
{
}

editor::QProjectTree::~QProjectTree()
{
}

editor::QProject* editor::QProjectTree::project() const
{
    return _project;
}

QString editor::QProjectTree::toString() const
{
    return _project->name();
}

