#pragma once
#include <scripts/angelscript_new/type.h>
#include <angelscript.h>

namespace editor::script
{

//! Defines a type which holds AngelScript objects.
class Function final
{
public:
    Function() noexcept = default;
    Function(ConstructorData<Function>) noexcept;

    ~Function() noexcept = default;

    //! \returns the property name.
    auto name() const noexcept -> std::string;

    //! \returns true If the function object is valid.
    bool valid() const noexcept { return _script_function != nullptr; }

    //! \returns The native AngelScript object.
    auto native() noexcept -> asIScriptFunction* { return _script_function; }

    //! \returns The native AngelScript object.
    auto native() const noexcept -> const asIScriptFunction* { return _script_function; }

    //! \returns a Function object from the script engine given the declaration.
    static auto from_index(const Engine& engine, uint32_t index) noexcept -> Function;

    //! \returns a Function object from the script engine given the declaration.
    static auto from_decl(const Engine& engine, const std::string& func_decl) noexcept -> Function;

    //! \returns a Function object from the script engine given the declaration.
    static auto from_native(asIScriptFunction* script_func) noexcept -> Function;

private:
    asIScriptFunction* _script_function;
};

} // namespace editor::script
