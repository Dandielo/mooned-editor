#include "QScriptedNodeEvent.h"

editor::QScriptedNodeEvent::QScriptedNodeEvent()
    : QEvent{ static_cast<QEvent::Type>(QScriptedNodeEvent::Type) }
{

}
