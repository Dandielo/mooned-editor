#include <scripts/angelscript_new/property.h>

#include "scripts/angelscript_new/private.h"

#include <cassert>

namespace editor::script
{

//////////////////////////////////////////////////////////////////////////

Property::Property() noexcept
    : _owning_type{ Type::invalid }
    , _prop_index{ std::numeric_limits<uint32_t>::max() }
    , _prop_name{ "<invalid>" }
    , _prop_type{ Type::invalid }
{ }

Property::Property(ConstructorData<Property> data) noexcept
    : _owning_type{ data.type }
    , _prop_index{ data.index }
    , _prop_name{ "<invalid>" }
    , _prop_type{ Type::invalid }
{
    typeid_t type_id;
    if (_prop_index < _owning_type.operator->()->GetPropertyCount())
    {
        _owning_type->GetProperty(_prop_index, &_prop_name, &type_id.value);
        _prop_type = Type::from_native(_owning_type->GetEngine()->GetTypeInfoById(type_id.value));
    }
}

auto Property::from_name(const Type& type, const std::string& property_name) noexcept -> Property
{
    Property result;
    for (const Property& prop : type.properties())
    {
        if (prop.name() == property_name)
        {
            result = prop;
            break;
        }
    }
    return result;
}

auto Property::from_index(const Type& type, uint32_t index) noexcept -> Property
{
    return ConstructorData<Property>{ type, index };
}

//! Defines the invalid property object.
const Property Property::invalid{ };

//////////////////////////////////////////////////////////////////////////

Type::Properties::Properties(Type type, uint32_t count) noexcept
    : _type{ std::move(type) }
    , _count{ count }
{ }

auto Type::Properties::begin() noexcept -> Iterator
{
    return Iterator{ _type, _count, 0 };
}

auto Type::Properties::end() noexcept -> Iterator
{
    return Iterator{ _type, _count, _count };
}

Type::Properties::Iterator::Iterator(Type type, uint32_t count, uint32_t index)
    : _type{ std::move(type) }
    , _count{ count }
    , _index{ index }
    , _current{ }
{
    _current = Property::from_index(_type, _index);
}

auto Type::Properties::Iterator::operator*() noexcept -> Property&
{
    assert(_index >= 0 && _index < _count);
    return _current;
}

auto Type::Properties::Iterator::operator*() const noexcept -> const Property&
{
    assert(_index >= 0 && _index < _count);
    return _current;
}

void Type::Properties::Iterator::operator++() noexcept
{
    ++_index;
    _current = Property::from_index(_type, _index);
}

void Type::Properties::Iterator::operator--() noexcept
{
    --_index;
    _current = Property::from_index(_type, _index);
}

bool Type::Properties::Iterator::operator==(Iterator& rhs) const noexcept
{
    assert(_type == rhs._type);
    return _index == rhs._index;
}

bool Type::Properties::Iterator::operator!=(Iterator& rhs) const noexcept
{
    return !(*this == rhs);
}

} // namespace editor::script
