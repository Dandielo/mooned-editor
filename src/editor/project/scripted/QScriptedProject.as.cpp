#include "QScriptedProject.h"

std::string asProjectGetNameProxy(editor::QScriptedProject* prj)
{
    return prj->name().toLocal8Bit().data();
}

void editor::QScriptedProject::registerTypeInterface(asIScriptEngine* engine)
{
    engine->RegisterObjectMethod(ClassName, "string get_name()", asFUNCTION(asProjectGetNameProxy), asCALL_CDECL_OBJFIRST);

    engine->RegisterInterface("IProject");
    engine->RegisterInterfaceMethod("IProject", "string get_name()");
    engine->RegisterInterfaceMethod("IProject", "array<string> AvailableElements()");

    engine->RegisterInterfaceMethod("IProject", "void OnCreate()");
    engine->RegisterInterfaceMethod("IProject", "void OnLoad()");
    engine->RegisterInterfaceMethod("IProject", "void OnSave()");
    engine->RegisterInterfaceMethod("IProject", "void OnExport()");
    engine->RegisterInterfaceMethod("IProject", "void OnClose()");
}
