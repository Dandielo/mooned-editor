#include "QScriptedProjectExporter.h"

void editor::QScriptedProjectExporter::registerTypeInterface(asIScriptEngine* engine)
{
    engine->RegisterInterface("IExporter");
    engine->RegisterInterfaceMethod("IExporter", "void OnSerialize(IProject@)");
}
