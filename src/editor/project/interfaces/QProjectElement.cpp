#include "QProjectElement.h"
#include "QProject.h"

#include <cassert>

editor::QProjectElement::QProjectElement(QProject* project)
    : QObject{ project }
    , _project{ project }
{
    assert(_project != nullptr);
}

editor::QProjectElement::~QProjectElement()
{
}

QString editor::QProjectElement::displayText() const
{
    return name();
}
