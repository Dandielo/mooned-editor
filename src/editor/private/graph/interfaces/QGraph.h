#pragma once 
#include "interfaces/QWorkspace.h"

#include "QNode.h"
#include "QGraphScene.h"
#include "QGraphView.h"

namespace editor
{
    class QGraph : public QWorkspace
    {
        Q_OBJECT;

    public:
        QGraph(QWidget* parent = nullptr);
        virtual ~QGraph() override = default;

        virtual QGraphicsView* view() const = 0;
        virtual QGraphicsScene* scene() const = 0;

        virtual void addNode(QNode* node) = 0;
        virtual void removeNode(QNode* node) = 0;
        virtual QVector<QNode*> nodes() const = 0;
    };
}

