#pragma once 
#include <QGraphicsScene>

namespace editor
{
    class QGraphScene : public QGraphicsScene
    {
        Q_OBJECT;

    public:
        QGraphScene();
        QGraphScene(const QRectF& rect);
        virtual ~QGraphScene() override = default;

    protected:
        virtual bool event(QEvent* ev) override;
    };
}

