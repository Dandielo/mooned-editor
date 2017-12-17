#include "QScriptedGraphSerializer.h"

#include "graph/interfaces/QGraph.h"
#include "graph/interfaces/QNode.h"
#include "graph/interfaces/QNodePin.h"
#include "graph/interfaces/QNodeConnection.h"

#include "graph/scripted/QScriptedGraph.h"
#include "graph/scripted/QScriptedNode.h"
#include "graph/scripted/QScriptedNodePin.h"
#include "graph/scripted/QScriptedNodePinValue.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QMap>

#include <cassert>

editor::QScriptedGraphSerializer::QScriptedGraphSerializer()
    : QGraphSerializer{ }
    , _script_manager{ nullptr }
{

}

editor::QScriptedGraphSerializer::~QScriptedGraphSerializer()
{
    _script_manager = nullptr;
}

void editor::QScriptedGraphSerializer::initialize(Scripts::CScriptManager* script_manager)
{
    _script_manager = script_manager;
}

bool editor::QScriptedGraphSerializer::serialize(QIODevice* io, QGraph* graph)
{
    QJsonArray json_node_array;

    const auto&& nodes = graph->nodes();
    for (QNode* basic_node : nodes)
    {
        auto* node = static_cast<QScriptedNode*>(basic_node);

        QJsonObject json_node;
        json_node.insert("name", { node->name() });
        json_node.insert("desc", { node->desc() });
        json_node.insert("class", { node->scriptType()->GetName() });
        json_node.insert("index", nodes.indexOf(node));

        QJsonArray json_pin_array;
        for (const QNodePin* basic_pin : node->inputPins())
        {
            auto* pin = static_cast<const QScriptedNodePin*>(basic_pin);

            QJsonObject json_pin;
            json_pin.insert("name", pin->name());
            json_pin.insert("color", pin->color().name(QColor::HexRgb));
            json_pin.insert("type", "input");
            json_pin.insert("value", pin->value()->value().toString());

            json_pin_array.append(json_pin);
        }

        for (const QNodePin* basic_pin : node->outputPins())
        {
            auto* pin = static_cast<const QScriptedNodePin*>(basic_pin);

            QJsonObject json_pin;
            json_pin.insert("name", pin->name());
            json_pin.insert("color", pin->color().name(QColor::HexRgb));
            json_pin.insert("type", "output");
            json_pin.insert("value", pin->value()->value().toString());

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

bool editor::QScriptedGraphSerializer::deserialize(QIODevice* io, QGraph* basic_graph)
{
    QByteArray data = io->readAll();
    QScriptedGraph* graph = static_cast<QScriptedGraph*>(basic_graph);

    // Parse the given document
    QJsonDocument json_project = QJsonDocument::fromJson(data);
    QJsonArray json_root = json_project.array();
    QMap<int, QScriptedNode*> nodes;

    uint count = json_root.count();
    for (uint i = 0; i < count; ++i)
    {
        QJsonObject json_node = json_root.at(i).toObject();
        QJsonObject json_metadata = json_node.value("editor_metadata").toObject();

        // Create the nodes
        auto* node = graph->newScriptNode(json_node.value("class").toString());
        node->setName(json_node.value("name").toString());
        node->setDesc(json_node.value("desc").toString());
        node->setPos(json_metadata.value("pos_x").toDouble(), json_metadata.value("pos_y").toDouble());

        nodes.insert(json_node.value("index").toInt(), node);

        // Set pin values
        QJsonArray json_pin_array = json_node.value("pins").toArray();
        uint pin_count = json_pin_array.count();
        for (uint j = 0; j < pin_count; ++j)
        {
            QJsonObject json_pin = json_pin_array.at(j).toObject();
            editor::PinType pin_type = json_pin.value("type").toString() == "input" ? PinType::Input : PinType::Output;

            QScriptedNodePin* pin = static_cast<QScriptedNodePin*>(node->getPin(pin_type, json_pin.value("name").toString()));
            if (nullptr != pin)
            {
                pin->value()->setValue(json_pin.value("value").toVariant());
            }
        }
    }

    auto it = nodes.begin();
    auto end = nodes.end();
    while(it != end)
    {
        QJsonObject json_node = json_root.at(it.key()).toObject();
        QScriptedNode* node = it.value();

        // Rebuild connections
        QJsonArray json_connection_array = json_node.value("connections").toArray();
        uint connection_count = json_connection_array.count();
        for (uint j = 0; j < connection_count; ++j)
        {
            QJsonObject json_conn = json_connection_array.at(j).toObject();
            QNodePin* output_pin = node->getPin(PinType::Output, json_conn.value("from").toString());

            QJsonObject json_conn_to = json_conn.value("to").toObject();
            QScriptedNode* target_node = nodes.value(json_conn_to.value("node_index").toInt());

            if (target_node && output_pin)
            {
                QNodePin* input_pin = target_node->getPin(PinType::Input, json_conn_to.value("pin").toString());

                if (input_pin)
                {
                    auto conn = output_pin->newNodeConnection(input_pin);
                    conn->updateConnection();
                    node->scene()->addItem(conn);
                }
            }
        }

        ++it;
    }
    return true;
}

