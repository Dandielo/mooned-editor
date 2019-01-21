#pragma once
#include <QGraphicsTextItem>

namespace editor
{
    class QGraphicsTextEdit : public QGraphicsTextItem
    {
        Q_OBJECT;

    public:
        QGraphicsTextEdit(QGraphicsItem* parent = nullptr);
        virtual ~QGraphicsTextEdit() override = default;

    signals:
        void typed(int key, bool& accept);
        void finished(QString value);

    protected:
        virtual void keyPressEvent(QKeyEvent* event) override;
        virtual void focusOutEvent(QFocusEvent *event) override;
    };
}
