#pragma once
#include <vector>
#include <unordered_map>

#include "angelscript/as_interpreter.h"

//using Scripts::AngelScript::AsScriptModule;

namespace Scripts
{

//! Defines a 'database' like object which can be queried for various things.
//! \note Queries to the database support annotated types and types with a parent.
class ScriptDatabase final
{
public:
    ScriptDatabase() noexcept;
    ~ScriptDatabase() noexcept = default;

    //! Extracts all data which can be stored from the given module.
    void extract_module(editor::script::Module& mod) noexcept;

    //! Queries the database for type objects and pushes the results into the given vector.
    //! \returns true If the query was successful.
    bool query_types(const std::string& query, std::vector<editor::script::Type>& results) const noexcept;

    //! Queries the database for type objects.
    //! \returns A vector of type objects.
    auto query_types(const std::string& query) const noexcept -> std::vector<editor::script::Type>;

    // Returns the value of a given attribute.
    auto get_attribute(editor::script::Type type, const std::string& attribute) const noexcept -> std::string;

    // Returns the value of a given attribute.
    auto get_attribute(editor::script::Property propery, const std::string& attribute) const noexcept -> std::string;

    // Pimpl like
    class CEntry;

protected:
    void extract_module_types(editor::script::Module& module) noexcept;
    void extract_module_type_properties(editor::script::Module& module, const editor::script::Type& type) noexcept;

private:
    using entry_list = std::vector<std::shared_ptr<CEntry>>;
    using type_map = std::unordered_map<std::string, editor::script::Type>;

    using type_metadata_map = std::unordered_map<std::string, entry_list>;
    using type_property_metadata_map = std::unordered_map<int, std::vector<std::unordered_map<std::string, std::string>>>;

    // The database data
    type_map _types{ };

    // Metadata maps
    type_metadata_map _type_metadata{ };
    type_property_metadata_map _property_metadata{ };
};

}
