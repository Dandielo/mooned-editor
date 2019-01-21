#pragma once
#include <scripts/angelscript_new/base.h>
#include <scripts/angelscript/as_script_module.h>

#include <angelscript.h>
#include <string>

namespace editor::script
{

//! The script engine type.
class Engine;

//! The script module type.
class Module;

//! The Method class declaration.
class Method;

//! The Method class declaration.
class Function;

//! The Property class declaration.
class Property;

//! The Property class declaration.
class Object;

//! Defines a type object which holds AngelScript native type object.
class Type final
{
public:
    Type() noexcept;
    Type(ConstructorData<Type>) noexcept;

    ~Type() noexcept = default;

    //! \returns The type id.
    auto id() const noexcept -> typeid_t;

    //! \returns The type name.
    auto name() const noexcept -> std::string;

    //! \returns true If the type object is valid.
    //! \note This means that the given type 'exists' in the engine.
    bool valid() const noexcept;

    //! Returns the native type object.
    auto native() noexcept -> asITypeInfo* { return _script_type; }

    //! Returns the native type object.
    auto native() const noexcept -> const asITypeInfo* { return _script_type; }

    //! Returns the type factory at the given index.
    auto factory_by_index(uint32_t index) const noexcept -> Function;

    //! Type properties proxy.
    class Properties;

    //! Returns a proxy object to walk over all properties.
    Properties properties() const noexcept;

    //! Type methods proxy.
    class Methods;

    //! Returns a proxy object to walk over all methods.
    Methods methods() const noexcept;

public:
    //! Allows to access the asITypeInfo interface.
    auto operator->() noexcept -> asITypeInfo* { return _script_type; }

    //! Allows to access the asITypeInfo interface.
    auto operator->() const noexcept -> const asITypeInfo* { return _script_type; }

    //! Allows to use this type in bool statements.
    operator bool() const noexcept { return valid(); }

    //! Creates a Type object from the script engine given the typeid.
    //! \note If a type of the given id does not exist, returns the 'invalid' type object.
    static auto from_id(const Engine& engine, typeid_t) noexcept -> Type;

    //! Creates a Type object from the script engine given the name.
    //! \note If a type of the given name does not exist, returns the 'invalid' type object.
    static auto from_name(const Engine& engine, const std::string& type_name) noexcept -> Type;

    //! Creates a Type object from the script engine given the native object.
    //! \note If the native object is a nullptr, returns the 'invalid' type object.
    static auto from_native(asITypeInfo* native_typeinfo) noexcept -> Type;

    //! Defines an default 'invalid' type object.
    static const Type invalid;

private:
    asITypeInfo* _script_type;
};

//! Defines comparison operators for the 'Type' class.
bool operator==(const Type& lhs, const Type& rhs) noexcept;
bool operator!=(const Type& lhs, const Type& rhs) noexcept;

bool operator==(const Type& lhs, typeid_t rhs) noexcept;
bool operator!=(const Type& lhs, typeid_t rhs) noexcept;

bool operator==(typeid_t lhs, const Type& rhs) noexcept;
bool operator!=(typeid_t lhs, const Type& rhs) noexcept;

//! Types iterator helper type.
class Module::Types final
{
public:
    class Iterator;

    ~Types() noexcept = default;

    auto begin() noexcept -> Iterator;
    auto end() noexcept -> Iterator;

public:
    class Iterator : public std::iterator<std::random_access_iterator_tag, Type>
    {
    public:
        auto operator*() noexcept -> Type&;
        auto operator*() const noexcept -> const Type&;

        void operator++() noexcept;
        void operator--() noexcept;

        bool operator==(Iterator& rhs) const noexcept;
        bool operator!=(Iterator& rhs) const noexcept;

    private:
        friend class Module::Types;

        Iterator(const asIScriptModule* module, uint32_t count, uint32_t index);

    private:
        const asIScriptModule* _module;
        const uint32_t _count;

        uint32_t _index;
        Type _current;
    };

private:
    friend class Module;
    Types(asIScriptModule*, uint32_t) noexcept;

private:
    const asIScriptModule* _module;
    const uint32_t _count;
};
}
