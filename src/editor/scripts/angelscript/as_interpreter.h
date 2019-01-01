#pragma once
#include <scripts/angelscript_new/object.h>

#include <memory>
#include <unordered_map>
#include <angelscript.h>

#include "as_script_module.h"
#include "as_context_pool.h"

namespace editor::script
{

//! Defines a proxy object for the AngelScript asIScriptEngine object.
//! \note This object holds the current execution state.
//! \note Every engine object WILL create a new state machine.
class Engine final
{
public:
    Engine() noexcept;
    ~Engine() noexcept = default;

    //! Returns the native engine object.
    auto native() noexcept -> asIScriptEngine* { return _script_engine.get(); }

    //! Returns the native engine object.
    auto native() const noexcept -> const asIScriptEngine* { return _script_engine.get(); }

    //! Returns the requested module, the module can be in a 'empty' state.
    auto find_module(const std::string& module_name) const noexcept -> Module;

    //! Returns the requested module, the module can be in a 'empty' state.
    auto get_module(const std::string& module_name, Module::Policy policy = Module::Policy::CreateIfMissing) noexcept -> Module;

    //! Tries to find the requested type.
    //! \returns Type::invalid on failure.
    auto get_type(const std::string& type_name) const noexcept -> Type;

    //! Creates a new object of the given type name.
    auto new_object(const std::string& type_name) noexcept -> Object;

    //! Creates a new object of the given type name.
    auto new_object(const Type& type) noexcept -> Object;


private:
    //! Deleter function signature.
    using script_engine_ptr = std::unique_ptr<asIScriptEngine, void(*)(asIScriptEngine*) noexcept>;

    //! Holds a unique pointer to a script engine instance.
    script_engine_ptr _script_engine;
};

} // namespace editor::script
