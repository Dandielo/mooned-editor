#include "QEditorEvent.h"

editor::QEditorEvent::QEditorEvent(editor::EventType type) : QEvent(static_cast<QEvent::Type>(type))
{

}
