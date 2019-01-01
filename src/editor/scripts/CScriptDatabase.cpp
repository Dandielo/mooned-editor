#include <scripts/CScriptDatabase.h>
#include <scripts/angelscript_new/type.h>
#include <scripts/angelscript_new/function.h>
#include <scripts/angelscript_new/property.h>


#include <cassert>
#include <algorithm>
#include <functional>

namespace Scripts
{
namespace detail
{

//! Extracts any type attribute definitions from metadata string.
bool extract_metadata_attributes(const std::string& metadata, std::unordered_map<std::string, std::string>& attributes) noexcept
{
    // Now find all attribute values from the beginning
    auto beg = metadata.begin();
    auto end = metadata.end();

    // Defines a helper function which allows to skip any whitespace and comma character.
    auto skip_ignored_characters = [end](auto& it) noexcept
    {
        while (it != end && (*it == ' ' || *it == ','))
        {
            ++it;
        }
    };

    // Skip the first batch of characters
    skip_ignored_characters(beg);

    // Look for attribute entries.
    std::vector<std::string> entries{ };
    while (beg != end)
    {
        auto it = beg;
        while (it != end && *it != ',') ++it; // Move past all characters that aren't ','

        // Push back a new value given the two iterators
        entries.push_back(std::string{ beg, it });

        // Skip all ignored characters and set a new beginning.
        skip_ignored_characters(it);
        beg = it;
    }

    bool found_any = !entries.empty();
    if (found_any)
    {
        // For each entry extract the key and value parts.
        std::for_each(entries.begin(), entries.end(), [&attributes](const std::string& entry)
            {
                // Basic values
                auto key_end = entry.find_first_of(':');
                auto value_beg = key_end;
                auto value_end = std::string::npos;

                // Fix the 'end' and beginning positions for both the key and value.
                if (value_beg != std::string::npos)
                {
                    key_end = entry.find_last_not_of(' ', value_beg - 1) + 1;
                    value_beg = entry.find_first_not_of(' ', value_beg + 1);
                    value_end = entry.find_last_not_of(' ') + 1;
                }
                else
                {
                    key_end = entry.find_last_not_of(' ') + 1;

                    value_beg = 0;
                    value_end = key_end;
                }

                // Add the key:value pair to the map
                attributes.emplace(entry.substr(0, key_end), entry.substr(value_beg, value_end - value_beg));
            });
    }

    return found_any;
}

//! Defines a set of errors possible to occur when extracting information for a type query.
enum class QueryError
{
    None,
    InvalidSyntax,
    InvalidCharacter,
};

//! Extracts any type attribute definitions from the given query string.
//! \returns An error if any occurred during the parsing.
auto query_extract_type_attributes(const std::string& query, std::unordered_map<std::string, std::string>& attributes) noexcept -> QueryError
{
    // Find the first and last bracket characters, which hold the attribute definitions.
    auto beg = query.find_first_of('[');
    auto end = query.find_last_of(']');

    // If we don't found the required characters just move the query to the result.
    if (beg == std::string::npos && end == std::string::npos)
    {
        return QueryError::None;
    }

    // Check the iterators (if the syntax 'matches')
    if (beg == std::string::npos || end == std::string::npos || beg > end)
    {
        return QueryError::InvalidSyntax;
    }

    // Get the attributes substring.
    auto clean_query = query.substr(beg + 1, end - beg - 1);
    if (clean_query.empty())
    {
        return QueryError::None;
    }

    extract_metadata_attributes(clean_query, attributes);
    return QueryError::None;
}

//! Extracts parent types from the query string
auto query_extract_parents(const std::string& query, std::vector<std::string>& parents) noexcept -> QueryError
{
    // Find the first and last bracket characters, which hold the attribute definitions.
    auto beg = query.find_last_of(':');

    // If we don't found the required characters just move the query to the result.
    if (beg == std::string::npos)
    {
        return QueryError::None;
    }

    auto clean_parents = query.substr(beg + 1);
    if (clean_parents.empty())
    {
        return QueryError::None;
    }

    auto it = clean_parents.begin();
    while (*it == ' ') ++it;

    auto end = std::prev(clean_parents.end());
    while (*end == ' ') --end;
    ++end; // Move the end forwards

    parents.push_back(std::string{ it, end });
    return QueryError::None;
}

//! Checks if there might be a simple single type to be queried for
auto query_extract_type(const std::string& query, std::string& type) noexcept -> QueryError
{
    // Find any semicolon character
    auto c1 = query.find_first_of(':');

    // Find any bracket opening or closing char
    auto c2 = query.find_first_of('[');
    auto c3 = query.find_first_of(']');

    if ((c1 != std::string::npos) || (c2 != std::string::npos) || (c3 != std::string::npos))
    {
        return QueryError::None;
    }

    auto beg = query.find_first_not_of(' ');
    auto end = query.find_last_not_of(' ');
    if (end == std::string::npos)
    {
        type = query.substr(beg);
    }
    else
    {
        type = query.substr(beg, end - beg + 1);
    }

    return QueryError::None;
}

} // namespace detail

class ScriptDatabase::CEntry abstract
{
public:
    CEntry(std::string value) : _value(value) { }
    virtual ~CEntry() { }

    std::string value() const
    {
        return _value;
    }

    virtual int id() const = 0;
    virtual auto type() const -> const editor::script::Type& = 0;

protected:
    std::string _value;
};

class CTypeEntry : public ScriptDatabase::CEntry
{
public:
    static constexpr int ID = 1;
    CTypeEntry(std::string value, const editor::script::Type& type) : CEntry(value), _type(type) { }

    int id() const override
    {
        return ID;
    }

    auto type() const -> const editor::script::Type& override
    {
        return _type;
    }

private:
    editor::script::Type _type;
};

// #todo Maybe a default ctor?
ScriptDatabase::ScriptDatabase() noexcept
{
}

void ScriptDatabase::extract_module(editor::script::Module& module) noexcept
{
    extract_module_types(module);
}

void ScriptDatabase::extract_module_types(editor::script::Module& script_module) noexcept
{
    for (const auto& type : script_module.types())
    {
        // Save the type in the map.
        _types.emplace(type.name(), type);

        // Extract attributes from the metadata string
        std::unordered_map<std::string, std::string> attributes;
        if (detail::extract_metadata_attributes(script_module.get_metadata(type), attributes))
        {
            std::for_each(attributes.begin(), attributes.end(), [this, &type](const auto& entry)
                {
                    _type_metadata[entry.first].push_back(std::make_shared<CTypeEntry>(entry.second, type));
                });
        }

        // Extract all type properties and their metadata.
        extract_module_type_properties(script_module, type);
    }
}

void ScriptDatabase::extract_module_type_properties(editor::script::Module& script_module, const editor::script::Type& type) noexcept
{
    //auto type = editor::script::Type::from_native(native_type);
    for (const auto& prop : type.properties())
    {
        // Extract all property metadata values
        std::string property_metadata = script_module.get_metadata(prop);

        // Extract attributes from the metadata string
        std::unordered_map<std::string, std::string> attributes;
        detail::extract_metadata_attributes(property_metadata, attributes);

        // We push empty values so we don't need fix indexes later.
        _property_metadata[type.id().value].push_back(attributes);
    }
}

auto ScriptDatabase::query_types(const std::string& query) const noexcept -> std::vector<editor::script::Type>
{
    std::vector<editor::script::Type> results;

    // Gather all attributes from the query string.
    std::unordered_map<std::string, std::string> attributes;
    if (detail::query_extract_type_attributes(query, attributes) != detail::QueryError::None)
    {
        assert(false);
    }

    // If no attributes where found take all types.
    if (attributes.empty())
    {
        std::for_each(_types.begin(), _types.end(), [&](const std::pair<const std::string, editor::script::Type>& type)
            {
                results.push_back(type.second);
            });
    }
    else
    {
        std::for_each(attributes.begin(), attributes.end(), [this, &results](const auto& attrib_entry)
            {
                // Skip if types with the given attrib do not exist
                if(_type_metadata.count(attrib_entry.first) == 0)
                {
                    return;
                }

                // Check each type entry
                const auto& types = _type_metadata.at(attrib_entry.first);
                std::for_each(types.begin(), types.end(), [&results, &attrib_entry](const auto& type_entry)
                    {
                        // Skip ?invalid types.
                        const auto& type_info = type_entry->type();
                        if (!type_info)
                        {
                            return;
                        }

                        if (type_entry->value() == attrib_entry.second)
                        {
                            results.push_back(type_info);
                        }
                    });
            });
    }

    // Extract parent types
    std::vector<std::string> parents;
    if (detail::query_extract_parents(query, parents) != detail::QueryError::None)
    {
        assert(false);
    }

    // If we have found any parent types we can continue
    if (!parents.empty())
    {
        // #todo we support only one parent type for now
        auto parent_type = _types.at(parents.front());
        if (parent_type)
        {
            // Find parents to be removed
            auto it = std::remove_if(results.begin(), results.end(), [&parent_type](const editor::script::Type& type)
                {
                    return parent_type != editor::script::Type::from_native(type.native()->GetBaseType());
                });

            // Erase types without the required parent
            if (it != results.end())
            {
                results.erase(it);
            }
        }
    }

    // Checks if we can just get away with checking the query as a type name.
    std::string type_name = "";
    detail::query_extract_type(query, type_name);
    if (!type_name.empty())
    {
        results.clear();

        const auto& script_type = _types.at(type_name);
        if (script_type)
        {
            results.push_back(script_type);
        }
    }

    return results;
}

std::string ScriptDatabase::get_attribute(editor::script::Type type, const std::string& attribute) const noexcept
{
    std::string result;
    if (_type_metadata.count(attribute))
    {
        const auto& types = _type_metadata.at(attribute);
        std::for_each(types.begin(), types.end(), [&](std::shared_ptr<CEntry> entry)
            {
                if (entry->type() == type)
                {
                    result = entry->value();
                }
            });
    }
    return result;
}

std::string Scripts::ScriptDatabase::get_attribute(editor::script::Property property, const std::string& attribute) const noexcept
{
    auto owner = property.owning_type();
    auto index = property.index();

    std::string result;
    if (_property_metadata.count(owner.id().value) > 0)
    {
        const auto& props = _property_metadata.at(owner.id().value);
        if (props[index].count(attribute) > 0)
            result = props[index].at(attribute);
    }
    return result;
}

} // namespace Scripts
