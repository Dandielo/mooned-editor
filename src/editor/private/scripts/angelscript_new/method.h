#pragma once
#include <scripts/angelscript_new/type.h>
#include <angelscript.h>

namespace editor::script
{

//! Defines a type which holds AngelScript objects.
class Method final
{
public:
    Method() noexcept = default;
    Method(ConstructorData<Method>) noexcept;

    ~Method() noexcept = default;

    //! \returns the property name.
    auto name() const noexcept -> std::string;

    //! \returns true If the function object is valid.
    bool valid() const noexcept { return _script_function != nullptr; }

    //! \returns The native AngelScript object.
    auto native() noexcept -> asIScriptFunction* { return _script_function; }

    //! \returns The native AngelScript object.
    auto native() const noexcept -> const asIScriptFunction* { return _script_function; }

    //! \returns a Function object from the script engine given the declaration.
    static auto from_index(const Type& owning_type, uint32_t index) noexcept -> Method;

    //! \returns a Function object from the script engine given the declaration.
    static auto from_name(const Type& owning_type, const std::string& func_name) noexcept -> Method;

    //! \returns a Function object from the script engine given the declaration.
    static auto from_decl(const Type& owning_type, const std::string& func_decl) noexcept -> Method;

private:
    Type _owning_type;
    asIScriptFunction* _script_function;
};

} // namespace editor::script
