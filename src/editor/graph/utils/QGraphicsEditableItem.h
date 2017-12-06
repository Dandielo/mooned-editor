#pragma once
#include <QGraphicsObject>

namespace editor
{
    class QGraphicsEditableItem : public QGraphicsObject
    {
        Q_OBJECT;

    public:
        QGraphicsEditableItem(QGraphicsItem* parent);
        virtual ~QGraphicsEditableItem() override = default;

        virtual void initialize() = 0;
        virtual void setColor(QColor color) = 0;

    public slots:
        virtual void setValue(QVariant value) = 0;

    signals:
        void changed(QVariant value, bool& accept);
        void finished(QVariant value);
    };
}
