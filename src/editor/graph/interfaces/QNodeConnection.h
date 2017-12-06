#pragma once 
#include <QGraphicsObject>

namespace editor
{
    class QNodePin;
    class QNodeConnection : public QGraphicsObject
    {
        Q_OBJECT;

    public:
        QNodeConnection();
        virtual ~QNodeConnection() override = default;

        virtual void updatePath();
        virtual void updateConnection() = 0;
        virtual void disconnect() = 0;

        virtual QNodePin* inputPin() const;
        virtual QNodePin* outputPin() const;

    protected:
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        QPainterPath _path;
        QColor _color;
    };
}
