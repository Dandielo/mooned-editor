#pragma once
#include <graph/interfaces/QGraphSerializer.h>

namespace editor
{

//! Graph serializer for script based graph objects.
class QScriptedGraphSerializer : public QGraphSerializer
{
public:
    //! Serializes the given graph to the given io device.
    bool serialize(QIODevice* io, const QGraph* graph) const noexcept override;

    //! De-serializes the given graph from the given io device.
    bool deserialize(QIODevice* io, QGraph* graph) noexcept override;
};

} // namespace editor
