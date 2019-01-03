#pragma once
#include <scripts/angelscript/as_interpreter.h>
#include <scripts/angelscript/as_context_pool.h>
#include <scripts/angelscript_new/function.h>
#include <scripts/CScriptDatabase.h>

#include <type_traits>
#include <cassert>

namespace editor::script
{

//! Helper struct to pass additional values to the factory function.
template<typename Type>
struct FactoryUserdata { /* empty by default */ };

} //namespace editor::script

namespace Scripts
{
using editor::script::Engine;
using editor::script::ScriptContext;

//! The owner of the script engine for the whole application.
class CScriptManager final
{
public:
    CScriptManager();
    ~CScriptManager() = default;

    //! Loads the given script file.
    void load(const std::filesystem::path& file) noexcept;

    //! \returns The AngelAcript engine object.
    auto engine() noexcept -> Engine& { return _engine; }

    //! \returns The AngelAcript engine object.
    auto engine() const noexcept -> const Engine& { return _engine; }

    //! \returns A script type with the given name.
    auto find_type(const std::string& name) const noexcept -> editor::script::Type;

    //! Searches the script database for types which match the given query string.
    //!   The query string consists of two parts which can be used separately:
    //!   - the 'annotation' list: '[tag1, tag2, tag3=asd]'
    //!   - the 'parent' check: ': ParentType'
    //! \note A query with both parts may look like this: '[special_type] : BaseClass'
    //! \returns A vector of types.
    auto query_types(const std::string& query) const noexcept -> std::vector<editor::script::Type>;

    asITypeInfo* GetTypeByName(const std::string& name) const;
    std::string GetTypeAttr(asITypeInfo* type, std::string attribute) const;
    std::string GetTypeAttr(asITypeInfo* type, std::string attribute, std::string default) const;
    std::string GetPropertyAttr(asITypeInfo* type, int index, std::string attribute) const;
    std::string GetPropertyAttr(asITypeInfo* type, int index, std::string attribute, std::string default) const;

    template<typename ObjectType, template<typename> typename UserdataType>
    ObjectType* create_object(const std::string& name, const UserdataType<ObjectType>& userdata = editor::script::FactoryUserdata<ObjectType>{ }) noexcept
    {
        auto object_type = _engine.find_module("engine").type_by_name(name);
        assert(object_type);

        auto object_factory = object_type.factory_by_index(0);
        assert(object_factory.valid());

        auto script_context = ScriptContext::from_any(object_factory);
        script_context.set_userdata(&userdata, 0x1);
        script_context.prepare(object_factory);
        script_context.call();

        auto script_object = reinterpret_cast<asIScriptObject*>(script_context.native()->GetReturnAddress());
        assert(script_object);

        auto* result = *reinterpret_cast<ObjectType**>(script_object->GetAddressOfProperty(0));
        assert(result);
        return result;
    }

    template<class T, int FlagValue = 0, class... Args>
    T* CreateObject(const std::string& name, Args&&... args)
    {
        auto typeinfo = _engine.find_module("engine").type_by_name(name);

        if (!typeinfo)
        {
            assert(!typeinfo);
            return nullptr;
        }

        // Get the factory
        auto factory = typeinfo.factory_by_index(0);

        T* result = nullptr;
        if (factory.valid())
        {
            // Get a context to call the factory function.
            auto context = editor::script::ScriptContext::from_any(typeinfo);
            context.set_userdata(reinterpret_cast<void*>(FlagValue), 0x01);

            // Prepare and call the function.
            context.prepare(factory);
            context.call(std::forward<Args>(args)...);

            auto script_object = reinterpret_cast<asIScriptObject*>(context.native()->GetReturnAddress());
            if (script_object != nullptr)
            {
                result = *reinterpret_cast<T**>(script_object->GetAddressOfProperty(0));
            }
        }

        return result;
    }

protected:
    void ProcessMetadata(std::string metadata, std::unordered_map<std::string, std::string>& data) const;

private:
    editor::script::Engine _engine{ };

    ScriptDatabase _database;
};

} // namespace editor
