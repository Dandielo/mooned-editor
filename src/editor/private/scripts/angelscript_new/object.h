#pragma once
#include <scripts/angelscript_new/type.h>
#include <angelscript.h>

namespace editor::script
{

//! Defines a type which holds AngelScript objects.
class Object final
{
public:
    Object() noexcept = default;
    Object(ConstructorData<Object>) noexcept;
    ~Object() noexcept = default;

    //! \returns true If the object is valid.
    bool valid() const noexcept { return _script_address != nullptr; }

    //! \returns The object address.
    auto address() noexcept -> void* { return _script_address; }

    //! \returns The object address.
    auto address() const noexcept -> const void* { return _script_address; }

private:
    Type _type{ Type::invalid };
    void* _script_address{ nullptr };
};

//! Defines a type which holds AngelScript objects.
class ScriptObject final
{
public:
    ScriptObject() noexcept = default;
    ScriptObject(ConstructorData<ScriptObject>) noexcept;

    //! Move constructors
    ScriptObject(ScriptObject&& other) noexcept;
    ScriptObject& operator=(ScriptObject&& other) noexcept;

    //! Copy constructors
    ScriptObject(const ScriptObject& other) noexcept;
    ScriptObject& operator=(const ScriptObject& other) noexcept;

    //! Dtor
    ~ScriptObject() noexcept;

    //! \returns true If the object is valid.
    bool valid() const noexcept { return _script_object != nullptr; }

    //! \returns The native AngelScript object.
    auto native() noexcept -> asIScriptObject* { return _script_object; }

    //! \returns The native AngelScript object.
    auto native() const noexcept -> const asIScriptObject* { return _script_object; }

    //! \returns The objects type.
    auto type() noexcept -> Type& { return _type; }

    //! \returns The objects type.
    auto type() const noexcept -> const Type& { return _type; }

    //! Creates a new ScriptObject object from a native object.
    static auto from_native(asIScriptObject* native_script_object) noexcept -> ScriptObject;

private:
    Type _type{ Type::invalid };
    asIScriptObject* _script_object{ nullptr };
};

} // namespace editor::script
