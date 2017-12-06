#pragma once 
#include "QEditorEventTypes.h"

namespace editor
{
    class QEditorEvent : public QEvent
    {
    public:
        QEditorEvent(EventType type);
        virtual ~QEditorEvent() override = default;
    };
}
