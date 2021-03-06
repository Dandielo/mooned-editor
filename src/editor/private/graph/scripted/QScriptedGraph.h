#pragma once
#include <graph/interfaces/QGraph.h>
#include <scripts/CNativeScriptObject.h>
#include <scripts/CScriptManager.h>

#include <QVector>

using Scripts::CNativeScriptObject;

namespace editor
{

class QScriptedNode;
class QScriptedGraphView;

class QScriptedGraph : public QGraph, public CNativeScriptObject<QScriptedGraph>
{
    Q_OBJECT;
    M_SCRIPT_TYPE(QScriptedGraph, "CGraph");

public:
    //! Alias for the script factory structure for this type.
    using FactoryData = editor::script::FactoryUserdata<QScriptedGraph>;

public:
    QScriptedGraph(editor::script::ScriptObject&& object);
    virtual ~QScriptedGraph() override;

    auto getScriptManager() const -> const auto* { return _script_manager; }

    void initialize(Scripts::CScriptManager* script_manager);
    void shutdown();

    virtual QString name() const override;
    virtual QGraphView* view() const override;
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
    QScriptedGraphView* _view;

    QVector<QNode*> _nodes;

private:
    friend class QScriptedGraphContextMenuHelper;
};

} // namespace editor
