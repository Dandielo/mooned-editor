#pragma once
#include "graph/interfaces/QGraph.h"
#include "scripts/CNativeScriptObject.h"

#include <QVector>

using Scripts::CNativeScriptObject;

namespace editor
{
    class QScriptedNode;

    class QScriptedGraph : public QGraph, public CNativeScriptObject<QScriptedGraph>
    {
        Q_OBJECT;
        M_SCRIPT_TYPE(QScriptedGraph, "CGraph");

    public:
        QScriptedGraph(asIScriptObject* object);
        virtual ~QScriptedGraph() override;

        void initialize(Scripts::CScriptManager* script_manager);

        virtual QString name() const override;
        virtual QGraphView* view() const override { return _view; }
        virtual QGraphScene* scene() const override { return _scene; }

        virtual void addNode(QNode* node) override;
        virtual void removeNode(QNode* node) override;
        virtual QVector<QNode*> nodes() const override;

    public slots:
        virtual QScriptedNode* newScriptNode(QString nodeclass);

    protected:
        Scripts::CScriptManager* _script_manager;
        asITypeInfo* _type;

        QGraphScene* _scene;
        QGraphView* _view;

        QVector<QNode*> _nodes;
    };
}
