#pragma once
#include "QGraphicsEditableItem.h"
#include "QGraphicsTextEdit.h"

namespace editor
{
    class QGraphicsLineEdit : public QGraphicsEditableItem
    {
        Q_OBJECT;

    public:
        QGraphicsLineEdit(QRectF rect, QGraphicsItem* parent);
        virtual ~QGraphicsLineEdit() override;

        virtual void initialize() override;

        virtual QRectF boundingRect() const override;
        virtual void setColor(QColor color) override;

    protected slots:
        void editCheck(int key, bool& accept);
        void editFinished(QString value);

    protected:
        virtual void setValue(QVariant value) override;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    private:
        const QRectF _rect;
        QGraphicsTextEdit _text_item;
    };
}
