#pragma once 
#include <QEvent>

namespace editor
{
    enum EventType
    {
        PinCreateConnection = QEvent::Type::User + 1,
        PinDestroyConnection = QEvent::Type::User + 2,
    };
}
