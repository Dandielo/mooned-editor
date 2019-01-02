#pragma once
#include <QIODevice>

namespace editor
{

//! The base QGraph  class.
class QGraph;

//! A interface for graph serialization for saving / loading purposes.
class QGraphSerializer
{
public:
    //! Ctor && Dtor
    QGraphSerializer() noexcept = default;
    virtual ~QGraphSerializer() noexcept = default;

    //! Serializes the given graph to the given io device.
    virtual bool serialize(QIODevice* io, const QGraph* graph) const noexcept = 0;

    //! De-serializes the given graph from the given io device.
    virtual bool deserialize(QIODevice* io, QGraph* graph) noexcept = 0;
};

} // namespace editor
