#pragma once
#include "graph/interfaces/QGraphSerializer.h"

namespace editor
{
    class QBasicGraphSerializer : public QGraphSerializer
    {
    public:
        QBasicGraphSerializer();
        virtual ~QBasicGraphSerializer() override;

        virtual bool serialize(QIODevice* io, QGraph* graph) override;
        virtual bool deserialize(QIODevice* io, QGraph* graph) override;
    };
}
