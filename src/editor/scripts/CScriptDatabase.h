#pragma once
#include <vector>
#include <unordered_map>

#include "angelscript/as_interpreter.h"

using Scripts::AngelScript::AsScriptModule;

namespace Scripts
{

    class CScriptDatabase final
    {
    public:
        CScriptDatabase();
        ~CScriptDatabase() = default;

        // Finds all types in the compiled module
        void AddModule(AsScriptModule::ptr mod);

        // Query functions 
        std::vector<asITypeInfo*> QueryTypes(std::string query) const;

        // Attribute functions 
        std::string GetTypeAttr(asITypeInfo* type, std::string attribute) const;
        std::string GetPropertyAttr(asITypeInfo* type, int index, std::string attribute) const;

        // Pimpl like 
        class CEntry;
            
    protected:
        void ExtractTypes(AsScriptModule::ptr module);
        void ExtractTypeProperties(AsScriptModule::ptr module, asITypeInfo* type);

    private:
        using entry_list = std::vector<std::shared_ptr<CEntry>>;
        using type_map = std::unordered_map<std::string, asITypeInfo*>;

        using type_metadata_map = std::unordered_map<std::string, entry_list>;
        using type_property_metadata_map = std::unordered_map<asITypeInfo*, std::vector<std::unordered_map<std::string, std::string>>>;

        // The database data
        type_map m_Types;

        // Metadata maps
        type_metadata_map m_TypeMetadata;
        type_property_metadata_map m_TypePropMetadata;
    };
}
