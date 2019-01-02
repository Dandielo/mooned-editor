#include "QScriptedElementGraph.h"

#include "graph/scripted/QScriptedGraph.h"
#include "project/scripted/QScriptedProject.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>


namespace editor
{

QScriptedElementGraph::QScriptedElementGraph(QScriptedProject* project, QString classname, QString name)
    : QProjectElement{ project, project->location().filePath("graphs/" + name.toLower() + "_" + classname.toLower() + ".mgraph") }
    , _class_name{ std::move(classname) }
{ }

QScriptedElementGraph::QScriptedElementGraph(QProjectElement* parent, QString classname, QFileInfo graph_file) noexcept
    : QProjectElement{ parent, graph_file }
    , _class_name{ std::move(classname) }
{ }

void QScriptedElementGraph::initialize(Scripts::CScriptManager* script_manager)
{
    _script_manager = script_manager;

    _graph = _script_manager->create_object(_class_name.toStdString(), editor::script::FactoryUserdata<QScriptedGraph>{ });
    _graph->initialize(_script_manager);
    _graph->setWindowTitle(value(Qt::ItemDataRole::DisplayRole).toString());
}

void QScriptedElementGraph::shutdown()
{
    if (nullptr != _graph)
    {
        _graph->shutdown();
        delete _graph;
    }
}

void QScriptedElementGraph::save() const noexcept
{
    QFile file{ fileinfo().absoluteFilePath() };
    file.open(QFile::Text | QFile::WriteOnly);

    if (file.isOpen())
    {
        QScriptedGraphSerializer{}.serialize(&file, _graph);
        file.close();
    }
}

void QScriptedElementGraph::load() noexcept
{
    QString path = fileinfo().absoluteFilePath();
    QFile file{ path };
    file.open(QFile::Text | QFile::ReadOnly);

    if (file.isOpen())
    {
        QScriptedGraphSerializer{}.deserialize(&file, _graph);
        file.close();
    }
}

auto QScriptedElementGraph::value(Qt::ItemDataRole role) const noexcept -> QVariant
{
    if (role == Qt::ItemDataRole::DisplayRole)
    {
        return QString{ "%1 {%2}" }.arg(name(), _class_name);
    }
    return { };
}

} // namespace editor
