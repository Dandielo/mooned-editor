#pragma once
#include "graph/interfaces/QNode.h"

namespace editor
{

class QBasicNode : public QNode
{
public:
    QBasicNode(QString name);
    ~QBasicNode();

    // Node
    void setColor(QColor color);
    void setName(QString name);
    void setDesc(QString description);

    void addPin(editor::PinType type, QString name);
    QNodePin* getPin(editor::PinType type, QString name);

    // Node properties
    QColor color() const override;
    QString name() const override;
    QString desc() const override;

    QVector<QNodePin*> inputPins() const override;
    QVector<QNodePin*> outputPins() const override;

private:
    QString _name;
    QString _description;
    QColor _color;

    QVector<QNodePin*> _input_pins;
    QVector<QNodePin*> _output_pins;
};

}