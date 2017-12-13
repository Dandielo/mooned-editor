#pragma once

#include <QIODevice>

namespace editor
{
    class QGraph;

    class QGraphSerializer
    {
    public:
        QGraphSerializer() = default;
        virtual ~QGraphSerializer() = default;

        virtual bool serialize(QIODevice* io, QGraph* graph) = 0;
        virtual bool deserialize(QIODevice* io, QGraph* graph) = 0;
    };
}