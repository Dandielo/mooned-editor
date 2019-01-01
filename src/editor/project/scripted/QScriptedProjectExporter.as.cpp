#include "QScriptedProjectExporter.h"

void editor::QScriptedProjectExporter::registerTypeInterface(asIScriptEngine* engine)
{
    engine->RegisterObjectMethod(ClassName, "void write(const string &in)", asMETHOD(QScriptedProjectExporter, write), asCALL_THISCALL);
    engine->RegisterObjectMethod(ClassName, "void writeln(const string &in)", asMETHOD(QScriptedProjectExporter, writeln), asCALL_THISCALL);
    engine->RegisterObjectMethod(ClassName, "void flush(const string &in)", asMETHOD(QScriptedProjectExporter, flush), asCALL_THISCALL);

    engine->RegisterInterface("IExporter");
    engine->RegisterInterfaceMethod("IExporter", "void OnExportProject(IProject@)");
    engine->RegisterInterfaceMethod("IExporter", "void OnExportGraph(IGraph@, const string &in)");
    //engine->RegisterInterfaceMethod("IExporter", "void OnSerialize(INode@)");
}
