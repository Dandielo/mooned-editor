#pragma once
#include <scripts/angelscript_new/type.h>

namespace editor::script
{

//! Defines a property proxy object which holds AngelScript objects beneath.
class Property final
{
public:
    Property() noexcept;
    Property(ConstructorData<Property>) noexcept;

    ~Property() noexcept = default;

    //! Returns the property name.
    auto name() const noexcept -> std::string { return _prop_name; }

    //! Returns the index of the property.
    auto index() const noexcept { return _prop_index; }

    //! Returns the property type.
    auto type() const noexcept -> Type { return _prop_type; }

    //! Returns the property owning type.
    auto owning_type() const noexcept -> Type { return _owning_type; }

    //! Creates a Property object from the script engine given the property name.
    static auto from_name(const Type& owning_type, const std::string& property_name) noexcept -> Property;

    //! Creates a Property object from the script engine given the property index.
    static auto from_index(const Type& owning_type, uint32_t index) noexcept -> Property;

    //! Defines an invalid property object.
    static const Property invalid;

private:
    Type _owning_type;
    uint32_t _prop_index;

    // Initialized later
    Type _prop_type;
    const char* _prop_name;
};

//! Properties iterator type.
class Type::Properties final
{
public:
    class Iterator;

    ~Properties() noexcept = default;

    auto begin() noexcept->Iterator;
    auto end() noexcept->Iterator;

public:
    class Iterator : public std::iterator<std::random_access_iterator_tag, Property>
    {
    public:
        auto operator*() noexcept->Property&;
        auto operator*() const noexcept -> const Property&;

        void operator++() noexcept;
        void operator--() noexcept;

        bool operator==(Iterator& rhs) const noexcept;
        bool operator!=(Iterator& rhs) const noexcept;

    private:
        friend class Type::Properties;

        Iterator(Type, uint32_t count, uint32_t index);

    private:
        const Type _type;
        const uint32_t _count;

        uint32_t _index;
        Property _current;
    };

private:
    friend class Type;
    Properties(Type, uint32_t) noexcept;

private:
    const Type _type;
    const uint32_t _count;
};

} // namespace editor::script
