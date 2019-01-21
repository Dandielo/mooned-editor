#include "as_interpreter.h"

#include <scriptarray.h>
#include <scriptstdstring.h>

#include "scripts/angelscript_new/private.h"

#include <QDebug>

namespace editor::script
{
namespace detail
{

//! The script engine deleter.
void release_script_engine(asIScriptEngine* engine) noexcept
{
    engine->Release();
}

//! The engine message callback
void engine_message_callback(const asSMessageInfo *msg, void *param)
{
    using logfunc_t = void(*)(const char* msg, ...);

    if (msg->type == asMSGTYPE_WARNING)
    {
        qDebug("[%s] %s (%d, %d): %s", "AngelScript::Compiler", msg->section, msg->row, msg->col, msg->message);
    }
    else if (msg->type == asMSGTYPE_INFORMATION)
    {
        qDebug("[%s] %s (%d, %d): %s", "AngelScript::Compiler", msg->section, msg->row, msg->col, msg->message);
    }
    else
    {
        qDebug("[%s] %s (%d, %d): %s", "AngelScript::Compiler", msg->section, msg->row, msg->col, msg->message);
    }
}

//! The engine message callback
void engine_print_function(const std::string& message)
{
    auto ctx = asGetActiveContext();
    std::string location = "AngelScript || ";
    if (ctx->GetThisPointer(0))
        location += reinterpret_cast<asIScriptObject*>(ctx->GetThisPointer())->GetObjectType()->GetName() + std::string("::");
    location += ctx->GetFunction(0)->GetName();

    qDebug("[%s] %s", location.c_str(), message.c_str());
}

} // namespace detail

//////////////////////////////////////////////////////////////////////////

Engine::Engine() noexcept
    : _script_engine{ asCreateScriptEngine(), &detail::release_script_engine }
{
    _script_engine->SetMessageCallback(asFUNCTION(detail::engine_message_callback), nullptr, asCALL_CDECL);


    // Register the string type
    RegisterStdString(_script_engine.get());
    RegisterScriptArray(_script_engine.get(), true);

    // Register a basic print function
    _script_engine->RegisterGlobalFunction("void print(const string& in)", asFUNCTION(detail::engine_print_function), asCALL_CDECL);
}

auto Engine::get_type(const std::string& type_name) const noexcept -> Type
{
    return Type::from_name(*this, type_name);
}

auto Engine::new_object(const std::string& type_name) noexcept -> Object
{
    return new_object(get_type(type_name));
}

auto Engine::new_object(const Type& type) noexcept -> Object
{
    if (type.valid())
    {
        return { }; // Creates an 'empty' object.
    }

    auto* const script_object = _script_engine->CreateScriptObject(type.native());
    if (script_object == nullptr)
    {
        return { }; // Creates an 'empty' object.
    }

    return ConstructorData<Object>{ type, script_object };
}

auto Engine::find_module(const std::string& module_name) const noexcept -> Module
{
    return Module::find(*this, module_name);
}

auto Engine::get_module(const std::string& module_name, Module::Policy policy /*= Module::Policy::CreateIfMissing*/) noexcept -> Module
{
    return Module::get(*this, module_name, policy);
}

} // namespace editor::script
