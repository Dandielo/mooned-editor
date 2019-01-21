#pragma once
#include <graph/interfaces/QGraphSerializer.h>

namespace editor
{

//! The script graph class.
class QScriptedGraph;

//! The script graph element class.
class QScriptedElementGraph;

//! Graph serializer for script based graph objects.
class QScriptedGraphSerializer final //: public QGraphSerializer
{
public:
    //! Serializes the given graph to the given io device.
    bool serialize(QIODevice* io, const QScriptedElementGraph* graph) const noexcept;

    //! De-serializes the given graph from the given io device.
    bool deserialize(QIODevice* io, QScriptedElementGraph* graph) noexcept;

private:
    //! Serializes the graph nodes.
    void serialize_nodes(QJsonArray& target, const QScriptedGraph* graph) const noexcept;

    //! De-serializes the graph nodes.
    void deserialize_nodes(const QJsonArray& target, QScriptedGraph* graph) noexcept;
};

} // namespace editor
