/*!
 *! This HEADER file is for INTERNAL use only, do not include it nor use the types defined in it by your own!!!
 *! \note Note this header file is to be included in specific .cpp files only!
 *! \todo Move this file to a 'private' project section
 */

#pragma once
#include <cinttypes>
#include <memory>

namespace editor::script
{

//! Base type predefinitions.
class Engine;
class Module;
class ModuleLoader;
class ScriptContext;
class Type;
class Function;
class Method;
class Property;
class Object;
class ScriptObject;

//! Module struct definition for constructor calling.
template<>
struct ConstructorData<Module>
{
    asIScriptModule* module;
    std::unique_ptr<ModuleLoader, void(*)(ModuleLoader*) noexcept> loader;
};

//! ScriptContext struct definition for constructor calling.
template<>
struct ConstructorData<ScriptContext>
{
    asIScriptContext* context;
};

//! Type struct definition for constructor calling.
template<>
struct ConstructorData<Type>
{
    asITypeInfo* typeinfo;
};

//! Type::TypeData struct definition for constructor calling.
template<>
struct ConstructorData<Function>
{
    asIScriptFunction* function;
};

//! Type::TypeData struct definition for constructor calling.
template<>
struct ConstructorData<Method>
{
    const Type& type;
    asIScriptFunction* function;
};

//! Property::PropertyData struct definition for constructor calling.
template<>
struct ConstructorData<Property>
{
    const Type& type;
    uint32_t index;
};

//! Type::TypeData struct definition for constructor calling.
template<>
struct ConstructorData<Object>
{
    const Type& type;
    void* object;
};

//! Type::TypeData struct definition for constructor calling.
template<>
struct ConstructorData<ScriptObject>
{
    const Type& type;
    asIScriptObject* object;
};

} // namespace editor::script
