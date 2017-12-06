#include "CScriptDatabase.h"

#include <cassert>
#include <algorithm>
#include <functional>

using namespace Scripts;

namespace Internal
{
    inline std::string trim(std::string str) {
        return str.substr(str.find_first_not_of(' '), str.find_last_not_of(' ') + 1);
    }

    inline void split(std::string metadata, char delim, std::vector<std::string>& out)
    {
        size_t at;
        while ((at = metadata.find_first_of(delim)) != std::string::npos)
        {
            out.emplace_back(metadata.substr(0, at)); 
            metadata = metadata.substr(at + 1);
        }  
        out.emplace_back(metadata);
    }

    inline void process_metadata(std::string metadata, std::unordered_map<std::string, std::string>& data)
    {
        if (metadata.empty()) return;

        std::vector<std::string> args;
        split(metadata, ',', args);

        for (auto& arg : args)
        {
            auto pos = arg.find_first_of(':');
            data.emplace(trim(arg.substr(0, pos)), trim(arg.substr(pos + 1)));
        }
    }
}

class CScriptDatabase::CEntry abstract
{
public:
    CEntry(std::string value) : m_Value(value) { }
    virtual ~CEntry() { }

    std::string value() const 
    {
        return m_Value;
    }

    virtual int id() const abstract;
    virtual asITypeInfo* getTypeinfo() const abstract;

protected:
    std::string m_Value;
};

class CTypeEntry : public CScriptDatabase::CEntry
{
public:
    static constexpr int ID = 1;
    CTypeEntry(std::string value, asITypeInfo* type) : CEntry(value), m_Type(type) { }

    int id() const override
    { 
        return ID;
    }

    asITypeInfo* getTypeinfo() const override
    {
        return m_Type;
    }

private:
    asITypeInfo* m_Type;
};

CScriptDatabase::CScriptDatabase() : m_Types(), m_TypeMetadata()
{
}

void CScriptDatabase::AddModule(AsScriptModule::ptr module)
{
    ExtractTypes(module);
}

void CScriptDatabase::ExtractTypes(AsScriptModule::ptr module)
{
    int types = module->GetObjectTypeCount();
    for (int i = 0; i < types; ++i)
    {
        std::string rawdata;
        auto type = module->GetObjectTypeByIndex(i, rawdata);

        // Cache the type
        m_Types.emplace(type->GetName(), type);

        // Parse the metadata string
        std::unordered_map<std::string, std::string> metadata;
        Internal::process_metadata(rawdata, metadata);

        // Add the results to the database
        for (auto& data : metadata)
        {
            auto entry = std::make_shared<CTypeEntry>(data.second, type);
            m_TypeMetadata[data.first].push_back(entry);
        }

        // Extract additional data
        ExtractTypeProperties(module, type);
    }
}

void CScriptDatabase::ExtractTypeProperties(AsScriptModule::ptr module, asITypeInfo* type)
{
    int property_count = type->GetPropertyCount();
    for (int index = 0; index < property_count; ++index)
    {
        std::string rawdata = module->GetMetadataForProperty(type, index);

        // Parse the metadata string
        std::unordered_map<std::string, std::string> metadata;
        Internal::process_metadata(rawdata, metadata);

        // Add the result
        m_TypePropMetadata[type].push_back(metadata);
    }
}

std::vector<asITypeInfo*> CScriptDatabase::QueryTypes(std::string query) const
{
    std::vector<asITypeInfo*> results;
    asITypeInfo* parent_type = nullptr;
    std::unordered_map<std::string, std::string> attributes;

    // Check for attribute requirements and find matching types
    std::string attribs = query.substr(query.find_first_of('[') + 1, query.find_last_of(']') - query.find_first_of('[') - 1);
    if (!attribs.empty())
    {
        Internal::process_metadata(attribs, attributes);
    }
    else
    {
        std::for_each(m_Types.begin(), m_Types.end(), [&](const std::pair<const std::string, asITypeInfo*>& type)
        {
            results.push_back(type.second);
        });
    }

    // If we got attribs add only valid types 
    assert(!m_TypeMetadata.empty());// , "No script types are registered in the database!");
    if (!m_TypeMetadata.empty())
    {
        std::for_each(attributes.begin(), attributes.end(), [&](std::pair<const std::string, std::string>& attr)
        {
            assert(m_TypeMetadata.count(attr.first) > 0);// , "No node types found! Editor will crash!");
            const auto& types = m_TypeMetadata.at(attr.first); // Get the types with the given attribute

                                                       // Check if we have that type in our result list and check if it's attribute value is same as the requested
            std::for_each(types.begin(), types.end(), [&](std::shared_ptr<CEntry> entry)
            {
                if (entry->value() == attr.second && entry->getTypeinfo() != nullptr)
                { 
                    results.push_back(entry->getTypeinfo());
                }
            });
        });
    }

    // Check for parent type requirement
    auto found = query.find_last_of(':') ;
    if (found != std::string::npos && found < query.size() - 1)
    {
        std::string parent = query.substr(found + 1);
        auto parent_type = m_Types.at(Internal::trim(parent));
        assert(parent_type);// , "Required parent types does not exist!");

        if (parent_type)
        {
            // Check the result types
            auto it = results.begin();
            while (it != results.end())
            {
                if ((*it)->GetBaseType() != parent_type)
                    it = results.erase(it);
                else ++it;
            }
        }
    }

    return results;
}

std::string CScriptDatabase::GetTypeAttr(asITypeInfo* type, std::string attribute) const
{
    std::string result;
    if (m_TypeMetadata.count(attribute))
    {
        const auto& types = m_TypeMetadata.at(attribute);
        std::for_each(types.begin(), types.end(), [&](std::shared_ptr<CEntry> entry)
        {
            if (entry->getTypeinfo() == type)
                result = entry->value();
        });   
    }
    return result;
}

std::string Scripts::CScriptDatabase::GetPropertyAttr(asITypeInfo* type, int index, std::string attr) const
{
    std::string result;
    if (m_TypePropMetadata.count(type) > 0)
    {
        const auto& props = m_TypePropMetadata.at(type);
        if (props[index].count(attr) > 0)
            result = props[index].at(attr);
    }
    return result;
}
