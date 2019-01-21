#include <scripts/angelscript_new/method.h>
#include <scripts/angelscript/as_interpreter.h>

#include "scripts/angelscript_new/private.h"

#include <cassert>

namespace editor::script
{

Method::Method(ConstructorData<Method> data) noexcept
    : _owning_type{ data.type }
    , _script_function{ data.function }
{ }

auto Method::name() const noexcept -> std::string
{
    return _script_function->GetName();
}

auto Method::from_index(const Type& owning_type, uint32_t index) noexcept -> Method
{
    ;
    return ConstructorData<Method>{ owning_type, owning_type.native()->GetMethodByIndex(index, true) };
}

auto Method::from_name(const Type& owning_type, const std::string& func_name) noexcept -> Method
{
    return ConstructorData<Method>{ owning_type, owning_type.native()->GetMethodByName(func_name.c_str(), true) };
}

auto Method::from_decl(const Type& owning_type, const std::string& func_decl) noexcept -> Method
{
    return ConstructorData<Method>{ owning_type, owning_type.native()->GetMethodByDecl(func_decl.c_str(), true) };
}

} // namespace editor::script
