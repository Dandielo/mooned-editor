#pragma once
#include "graph/interfaces/QGraphView.h"

namespace editor
{
    class QBasicGraphView : public QGraphView
    {
        Q_OBJECT;

    public:
        QBasicGraphView(QWidget* parent = nullptr);
        virtual ~QBasicGraphView() override = default;
    };
}
