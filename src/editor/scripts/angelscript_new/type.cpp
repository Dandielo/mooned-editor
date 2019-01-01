#include <scripts/angelscript_new/type.h>
#include <scripts/angelscript_new/property.h>
#include <scripts/angelscript_new/function.h>

#include <scripts/angelscript/as_interpreter.h>
#include <scripts/angelscript/as_script_module.h>

#include "scripts/angelscript_new/private.h"

#include <cassert>

namespace editor::script
{
namespace detail
{

//! The none type info object.
class NoneTypeInfo : public asITypeInfo
{
public:
    NoneTypeInfo() noexcept = default;

    //! Returns the associated script engine, this means NONE.
    asIScriptEngine* GetEngine() const noexcept override { return nullptr; }

    //! Returns always nullptr, because this type does not exists.
    asIScriptModule* GetModule() const noexcept override { return nullptr; }

    //! Returns an really invalid type name.
    const char* GetName() const noexcept override { return "<invalid>"; }

    //! Returns always an empty string.
    const char* GetNamespace() const noexcept override { return ""; }

    //! Returns a nullptr for the base type.
    asITypeInfo * GetBaseType() const noexcept override { return nullptr; }

    //! Returns a 0 for the size, as because returning -1 would be casted to an 'unsigned int' which results in a huge value.
    asUINT GetSize() const noexcept override { return 0; }

    //! Returns the same type id as the 'void' type because one might think, those are the same and in some cases they are.
    int GetTypeId() const noexcept override { return asTYPEID_VOID; }

    //! #todo Should it return the same value like GetTypeId?
    int GetTypedefTypeId() const noexcept override { return GetTypeId(); }

    // Empty implementations.
    const char* GetConfigGroup() const noexcept override { return ""; }
    asDWORD GetAccessMask() const noexcept override { return 0; }


    int AddRef() const noexcept override { return -1; }
    int Release() const noexcept override { return -1; }


    bool DerivesFrom(const asITypeInfo* /* objType */) const noexcept override { return false; }


    asDWORD GetFlags() const noexcept override { return 0; }


    asUINT GetSubTypeCount() const noexcept override { return 0; }
    asITypeInfo* GetSubType(asUINT /* subTypeIndex = 0*/) const noexcept override { return nullptr; }
    int GetSubTypeId(asUINT /*subTypeIndex = 0*/) const noexcept override { return 0; }


    asUINT GetInterfaceCount() const noexcept override { return 0; }
    asITypeInfo* GetInterface(asUINT /*index*/) const noexcept override { return nullptr; }


    bool Implements(const asITypeInfo* /*objType*/) const noexcept override { return false; }


    asUINT GetFactoryCount() const noexcept override { return 0; }
    asIScriptFunction* GetFactoryByIndex(asUINT /*index*/) const noexcept override { return nullptr; }
    asIScriptFunction* GetFactoryByDecl(const char* /*decl*/) const noexcept override { return nullptr; }


    asUINT GetMethodCount() const noexcept override { return 0; }
    asIScriptFunction* GetMethodByIndex(asUINT /*index*/, bool /*getVirtual = true*/) const noexcept override { return nullptr; }
    asIScriptFunction* GetMethodByName(const char* /*name*/, bool /*getVirtual = true*/) const noexcept override { return nullptr; }
    asIScriptFunction* GetMethodByDecl(const char* /*decl*/, bool /*getVirtual = true*/) const noexcept override { return nullptr; }


    asUINT GetPropertyCount() const noexcept override { return 0; }
    int GetProperty(asUINT, const char**, int*, bool*, bool*, int*, bool*, asDWORD*) const noexcept override { return 0; }
    const char* GetPropertyDeclaration(asUINT /*index*/, bool /*includeNamespace = false*/) const noexcept override { return nullptr; }


    asUINT GetBehaviourCount() const noexcept override { return 0; }
    asIScriptFunction* GetBehaviourByIndex(asUINT /*index*/, asEBehaviours* /*outBehaviour*/) const noexcept override { return nullptr; }


    asUINT GetChildFuncdefCount() const noexcept override { return 0; }
    asITypeInfo* GetChildFuncdef(asUINT /*index*/) const noexcept override { return nullptr; }


    asITypeInfo* GetParentType() const noexcept override { return nullptr; }


    asUINT GetEnumValueCount() const noexcept override { return 0; }
    const char* GetEnumValueByIndex(asUINT /*index*/, int* /*outValue*/) const noexcept override { return nullptr; }


    asIScriptFunction* GetFuncdefSignature() const noexcept override { return nullptr; }


    void* SetUserData(void* /*data*/, asPWORD /*type = 0*/) noexcept override { return nullptr; }
    void* GetUserData(asPWORD /*type = 0*/) const noexcept override { return nullptr; }
};

// A static value of the NoneTypeInfo object.
static NoneTypeInfo none_type_typeinfo;

} // namespace detail

//////////////////////////////////////////////////////////////////////////

Type::Type() noexcept
    : _script_type{ &detail::none_type_typeinfo }
{ }

Type::Type(ConstructorData<Type> data) noexcept
    : _script_type{ data.typeinfo }
{ }

auto Type::id() const noexcept -> typeid_t
{
    assert(_script_type != nullptr);
    return { _script_type->GetTypeId() };
}

auto Type::name() const noexcept -> std::string
{
    assert(_script_type != nullptr);
    return _script_type->GetName();
}

bool Type::valid() const noexcept
{
    return _script_type != &detail::none_type_typeinfo;
}

auto Type::factory_by_index(uint32_t index) const noexcept -> Function
{
    return Function::from_native(_script_type->GetFactoryByIndex(index));
}

Type::Properties Type::properties() const noexcept
{
    return Type::Properties{ *this, _script_type->GetPropertyCount() };
}

auto Type::from_id(const Engine& engine, typeid_t type_id) noexcept->Type
{
    return from_native(engine.native()->GetTypeInfoById(type_id.value));
}

auto Type::from_name(const Engine& engine, const std::string& type_name) noexcept -> Type
{
    return from_native(engine.native()->GetTypeInfoByName(type_name.c_str()));
}

auto Type::from_native(asITypeInfo* script_typeinfo) noexcept -> Type
{
    if (script_typeinfo == nullptr)
    {
        script_typeinfo = &detail::none_type_typeinfo;
    }
    return ConstructorData<Type>{ script_typeinfo };
}

// Defines the 'invalid_type' object, which holds the same ID as the 'void' type but returns another name and will not be equal with the void type.
const Type Type::invalid{ ConstructorData<Type>{ &detail::none_type_typeinfo } };

//////////////////////////////////////////////////////////////////////////

// Type == Type
bool operator==(const Type& lhs, const Type& rhs) noexcept
{
    return lhs.operator->() == rhs.operator->();
}

// Type != Type
bool operator!=(const Type& lhs, const Type& rhs) noexcept
{
    return !(lhs == rhs);
}

// Type == typeid_t
bool operator==(const Type& lhs, typeid_t rhs) noexcept
{
    return lhs.id() == rhs;
}

// Type != typeid_t
bool operator!=(const Type& lhs, typeid_t rhs) noexcept
{
    return !(lhs == rhs);
}

// typeid_t == Type
bool operator==(typeid_t lhs, const Type& rhs) noexcept
{
    return rhs == lhs;
}

// typeid_t != Type
bool operator!=(typeid_t lhs, const Type& rhs) noexcept
{
    return rhs != lhs;
}

//////////////////////////////////////////////////////////////////////////

auto Module::Types::begin() noexcept -> Iterator
{
    return Iterator{ _module, _count, 0 };
}

auto Module::Types::end() noexcept -> Iterator
{
    return Iterator{ _module, _count, _count };
}

auto Module::Types::Iterator::operator*() noexcept -> Type&
{
    assert(_index >= 0 && _index < _count);
    return _current;
}

auto Module::Types::Iterator::operator*() const noexcept -> const Type&
{
    assert(_index >= 0 && _index < _count);
    return _current;
}

void Module::Types::Iterator::operator++() noexcept
{
    ++_index;
    _current = Type::from_native(_module->GetObjectTypeByIndex(_index));
}

void Module::Types::Iterator::operator--() noexcept
{
    --_index;
    _current = Type::from_native(_module->GetObjectTypeByIndex(_index));
}

bool Module::Types::Iterator::operator==(Iterator& rhs) const noexcept
{
    assert(_module == rhs._module);
    return _index == rhs._index;
}

bool Module::Types::Iterator::operator!=(Iterator& rhs) const noexcept
{
    return !(*this == rhs);
}

Module::Types::Iterator::Iterator(const asIScriptModule* script_module, uint32_t count, uint32_t index)
    : _module{ script_module }
    , _count{ count }
    , _index{ index }
    , _current{ }
{
    _current = Type::from_native(script_module->GetObjectTypeByIndex(_index));
}

Module::Types::Types(asIScriptModule* script_module, uint32_t count) noexcept
    : _module{ script_module }
    , _count{ count }
{ }

} // namespace editor::script
