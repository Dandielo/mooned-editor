#pragma once

#include <QEvent>

namespace editor
{
    class QScriptedNodeEvent : public QEvent
    {
    public:
        QScriptedNodeEvent();
        virtual ~QScriptedNodeEvent() override = default;

    public:
        static constexpr int Type = QEvent::Type::User + 1;
    };
}
