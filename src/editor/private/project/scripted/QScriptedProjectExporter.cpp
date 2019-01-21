#include "QScriptedProjectExporter.h"
#include "QScriptedProject.h"

#include "project/scripted/elements/QScriptedElementGraph.h"
#include "graph/scripted/QScriptedGraph.h"

#include <iostream>

namespace editor
{

QScriptedProjectExporter::QScriptedProjectExporter(editor::script::ScriptObject&& object)
    : QProjectExporter{ }
    , CNativeScriptObject{ std::move(object) }
{
    _stream.clear();
}

// Save the exported results?
QScriptedProjectExporter::~QScriptedProjectExporter()
{
}

void QScriptedProjectExporter::export_project(QProject* project) const noexcept
{
    auto* const scripted_project = dynamic_cast<editor::QScriptedProject*>(project);
    if (scripted_project)
    {
        CallScriptMethod("OnExportProject", scripted_project->script_object());
    }
}

void QScriptedProjectExporter::export_project_element(QProjectElement* graph) const noexcept
{
    auto* const scripted_graph_element = dynamic_cast<editor::QScriptedElementGraph*>(graph);
    if (scripted_graph_element)
    {
        CallScriptMethod("OnExportGraph", scripted_graph_element->graph()->script_object(), scripted_graph_element->name().toStdString());
    }
}

void QScriptedProjectExporter::write(const std::string& str) noexcept
{
    _stream << str;
}

void QScriptedProjectExporter::writeln(const std::string& str) noexcept
{
    _stream << str << "\n";
}

void QScriptedProjectExporter::flush(const std::string& file_name) noexcept
{
    std::cout << "\nSaving " << file_name << " with content:\n" << _stream.str() << std::endl;
    _stream.clear();
}

//void QScriptedProjectExporter::serialize(QScriptedNode* node) const noexcept
//{
//
//}

} // namespace editor
