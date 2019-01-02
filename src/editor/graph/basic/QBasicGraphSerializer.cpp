#include "QBasicGraphSerializer.h"

#include "graph/interfaces/QGraph.h"
#include "graph/interfaces/QNode.h"
#include "graph/interfaces/QNodePin.h"
#include "graph/interfaces/QNodeConnection.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <cassert>

namespace editor
{

bool QBasicGraphSerializer::serialize(QIODevice* io, const QGraph* graph) const noexcept
{
    QJsonArray json_node_array;

    const auto&& nodes = graph->nodes();
    for (QNode* node : nodes)
    {
        QJsonObject json_node;
        json_node.insert("name", { node->name() });
        json_node.insert("desc", { node->desc() });
        json_node.insert("index", nodes.indexOf(node));

        QJsonArray json_pin_array;
        for (const QNodePin* pin : node->inputPins())
        {
            QJsonObject json_pin;
            json_pin.insert("name", pin->name());
            json_pin.insert("color", pin->color().name(QColor::HexRgb));
            json_pin.insert("type", "input");

            json_pin_array.append(json_pin);
        }

        for (const QNodePin* pin : node->outputPins())
        {
            QJsonObject json_pin;
            json_pin.insert("name", pin->name());
            json_pin.insert("color", pin->color().name(QColor::HexRgb));
            json_pin.insert("type", "output");

            json_pin_array.append(json_pin);
        }

        json_node.insert("pins", json_pin_array);


        QJsonArray json_connection_array;

        for (const QNodePin* pin : node->outputPins())
        {
            for (const QNodeConnection* conn : pin->connections())
            {
                QJsonObject json_conn;
                json_conn.insert("from", pin->name());

                auto* target_node = dynamic_cast<QNode*>(conn->inputPin()->parent());
                assert(target_node != nullptr);

                QJsonObject json_conn_to;
                json_conn_to.insert("node_index", nodes.indexOf(target_node));
                json_conn_to.insert("pin", conn->inputPin()->name());
                json_conn.insert("to", json_conn_to);

                json_connection_array.append(json_conn);
            }
        }

        json_node.insert("connections", json_connection_array);

        QJsonObject editor_metadata;
        editor_metadata.insert("pos_x", node->pos().x());
        editor_metadata.insert("pos_y", node->pos().y());

        json_node.insert("editor_metadata", editor_metadata);
        json_node_array.append(json_node);
    }

    // generate the resulting document
    QJsonDocument json_graph(json_node_array);
    io->write(json_graph.toJson());
    return true;
}

bool QBasicGraphSerializer::deserialize(QIODevice* io, QGraph* graph) noexcept
{
    return true;
}

} // namespace editor
