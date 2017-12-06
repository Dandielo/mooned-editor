#pragma once 
#include "graph/interfaces/QGraphScene.h"

namespace editor
{
    class QBasicGraphScene : public QGraphScene
    {
        Q_OBJECT;

    public:
        QBasicGraphScene(const QRectF& rect);
        virtual ~QBasicGraphScene() override = default;
    };
}