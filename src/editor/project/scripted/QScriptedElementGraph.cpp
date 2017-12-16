#include "QScriptedElementGraph.h"
#include "QScriptedProject.h"

#include "graph/scripted/QScriptedGraph.h"

editor::QScriptedElementGraph::QScriptedElementGraph(QScriptedProject* project, QString classname, QString name)
    : QProjectElement{ project }
    , _script_manager{ nullptr }
    , _graph{ nullptr }
    , _classname{ classname }
    , _name{ name }
{
}

editor::QScriptedElementGraph::~QScriptedElementGraph()
{
}

void editor::QScriptedElementGraph::initialize(Scripts::CScriptManager* script_manager)
{
    _script_manager = script_manager;
    _graph = _script_manager->CreateObject<editor::QScriptedGraph, 1>(_classname.toLocal8Bit().data());
    _graph->initialize(_script_manager);
    _graph->setWindowTitle(_name);
}

void editor::QScriptedElementGraph::shutdown()
{
    _graph->shutdown();
    delete _graph;
}

QString editor::QScriptedElementGraph::name() const
{
    return QString("%1::%2").arg(_classname, _name);
}

QString editor::QScriptedElementGraph::displayText() const
{
    return QString("%1 { %2 }").arg(_name, _classname);
}

