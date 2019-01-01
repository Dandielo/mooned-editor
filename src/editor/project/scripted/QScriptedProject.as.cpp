#include "QScriptedProject.h"

std::string asProjectGetNameProxy(editor::QScriptedProject* prj)
{
    return prj->name().toLocal8Bit().data();
}

namespace editor
{
namespace detail
{

QScriptedProject* scripted_project_factory()
{
    auto ctx = asGetActiveContext();

    // Get the factory data object
    auto udata = reinterpret_cast<const QScriptedProject::FactoryData*>(ctx->GetUserData(0x01));
    assert(udata != nullptr);

    auto func = ctx->GetFunction(0);
    if (func->GetObjectType() == 0 || strcmp(func->GetObjectType()->GetName(), "CProject") != 0)
    {
        std::string msg = "Invalid attempt to manually instantiate ";
        msg += "CProjectNative";
        ctx->SetException(msg.c_str());
        return nullptr;
    }

    auto obj = reinterpret_cast<asIScriptObject*>(ctx->GetThisPointer(0));
    return new QScriptedProject{ editor::script::ScriptObject::from_native(obj), udata->file, udata->type };
}

} // namespace detail
} // namespace editor

void editor::QScriptedProject::registerTypeInterface(asIScriptEngine* engine)
{
    engine->RegisterObjectMethod(ClassName, "string get_name()", asFUNCTION(asProjectGetNameProxy), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectBehaviour(ClassName, asBEHAVE_FACTORY, "CProjectNative@ f()", asFUNCTION(detail::scripted_project_factory), asCALL_CDECL);

    engine->RegisterInterface("IProject");
    engine->RegisterInterfaceMethod("IProject", "string get_name()");
    engine->RegisterInterfaceMethod("IProject", "array<string> AvailableElements()");

    engine->RegisterInterfaceMethod("IProject", "void OnCreate()");
    engine->RegisterInterfaceMethod("IProject", "void OnLoad()");
    engine->RegisterInterfaceMethod("IProject", "void OnSave()");
    engine->RegisterInterfaceMethod("IProject", "void OnExport()");
    engine->RegisterInterfaceMethod("IProject", "void OnClose()");
}
