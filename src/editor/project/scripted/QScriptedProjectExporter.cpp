#include "QScriptedProjectExporter.h"
#include "QScriptedProject.h"

editor::QScriptedProjectExporter::QScriptedProjectExporter(asIScriptObject* object)
    : QProjectExporter{ }
    , CNativeScriptObject{ object }
{
}

editor::QScriptedProjectExporter::~QScriptedProjectExporter()
{
}

void editor::QScriptedProjectExporter::serialize(QProject* project)
{
     auto* scripted_project = dynamic_cast<editor::QScriptedProject*>(project);
     if (nullptr == scripted_project)
     {
         CallScriptMethod("OnSerialize", scripted_project->ScriptObject());
     }
}
