#include <scripts/angelscript_new/function.h>
#include <scripts/angelscript/as_interpreter.h>

#include "scripts/angelscript_new/private.h"

#include <cassert>

namespace editor::script
{

Function::Function(ConstructorData<Function> data) noexcept
    : _script_function{ data.function }
{ }

auto Function::name() const noexcept -> std::string
{
    return _script_function->GetName();
}

auto Function::from_index(const Engine& engine, uint32_t index) noexcept -> Function
{
    return ConstructorData<Function>{ engine.native()->GetGlobalFunctionByIndex(index) };
}

auto Function::from_decl(const Engine& engine, const std::string& func_decl) noexcept -> Function
{
    return ConstructorData<Function>{ engine.native()->GetGlobalFunctionByDecl(func_decl.c_str()) };
}

auto Function::from_native(asIScriptFunction* script_func) noexcept -> Function
{
    return ConstructorData<Function>{ script_func };
}

} // namespace editor::script
