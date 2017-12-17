#include "QScriptedGraph.h"
#include "graph/basic/QBasicGraphScene.h"
#include "graph/scripted/QScriptedGraphView.h"
#include "graph/scripted/QScriptedNode.h"

#include "scripts/CScriptManager.h"

#include <QOpenGLWidget>
#include <QGridLayout>
#include <QGLWidget>

#include <QDebug>

editor::QScriptedGraph::QScriptedGraph(asIScriptObject* obj)
    : QGraph{ }
    , CNativeScriptObject{ obj }
    , _script_manager{ nullptr }
    , _type{ nullptr }
    , _scene{ nullptr }
    , _view{ nullptr }
    , _nodes{ }
{
    _type = obj->GetObjectType();

    _scene = new QBasicGraphScene({ 0, 0, 10000, 10000 });
    _scene->setParent(this);

    // View settings
    _view = new QScriptedGraphView{};
    _view->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    _view->setViewport(new QOpenGLWidget());
    _view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    _view->setInteractive(true);
    //_view->setCacheMode(QGraphicsView::CacheBackground);
    _view->setScene(_scene);

    // Set dock widget properties
    setWindowTitle(name());

    // Last initializations
    QWidget* centralWidget = new QWidget(this);
    QLayout* layout = new QGridLayout(centralWidget);
    layout->setMargin(0);
    layout->addWidget(_view);

    setWidget(centralWidget);
}

editor::QScriptedGraph::~QScriptedGraph()
{
}

void editor::QScriptedGraph::initialize(Scripts::CScriptManager* script_manager)
{
    _script_manager = script_manager;
    _view->initialzie(script_manager, this);

    // Sets the default title
    setWindowTitle(QString::fromStdString(_script_manager->GetTypeAttr(_type, "name")));

    // Finish the initialization process
    CallScriptMethod("OnCreate");
}

void editor::QScriptedGraph::shutdown()
{
    CallScriptMethod("OnDestroy");

    _view->shutdown();

    for (auto* node : nodes())
    {
        auto* snode = dynamic_cast<editor::QScriptedNode*>(node);
        if (snode)
        {
            snode->shutdown();
        }
        delete node;
    }
}

QString editor::QScriptedGraph::name() const
{
    return _type->GetName();
}

editor::QGraphView* editor::QScriptedGraph::view() const
{
    return _view;
}

void editor::QScriptedGraph::addNode(QNode* node)
{
    auto* script_node = dynamic_cast<editor::QScriptedNode*>(node);
    if (script_node)
    {
        _nodes.push_back(script_node);
        _scene->addItem(script_node);

        // When a node is added
        CallScriptMethod("OnNodeAdded", script_node->ScriptObject());
    }
}

void editor::QScriptedGraph::removeNode(QNode* node)
{
    auto* script_node = dynamic_cast<editor::QScriptedNode*>(node);
    if (script_node)
    {
        _scene->removeItem(script_node);
        _nodes.removeAt(_nodes.indexOf(script_node));

        // When a node is removed
        CallScriptMethod("OnNodeRemoved", script_node->ScriptObject());
        script_node->deleteLater();
    }
}

QVector<editor::QNode*> editor::QScriptedGraph::nodes() const
{
    return _nodes;
}

editor::QScriptedNode* editor::QScriptedGraph::newScriptNode(QString nodeclass)
{
    auto* node = _script_manager->CreateObject<editor::QScriptedNode, 1>(nodeclass.toLocal8Bit().data(), false);
    if (node != nullptr)
    {
        node->initialize(_script_manager);
        addNode(node);
    }
    return node;
}
