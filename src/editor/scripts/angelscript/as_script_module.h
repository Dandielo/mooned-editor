#pragma once
#include <scripts/angelscript_new/base.h>

#include <angelscript.h>
#include <filesystem>
#include <memory>

namespace editor::script
{

//! The script engine class.
class Engine;

//! The script loader internal class.
class ModuleLoader;

//! The script type class.
class Type;

//! The script property class.
class Property;

//! A wrapper for asIScriptModule objects.
class Module final
{
public:
    enum class Policy { AlwaysCreate, CreateIfMissing, FindOnly };

    Module(ConstructorData<Module>) noexcept;
    ~Module() noexcept = default;

    //! \returns true If the module is valid.
    bool valid() const noexcept { return _script_module != nullptr; }

    //! \returns The native script module object.
    auto native() noexcept -> asIScriptModule* { return _script_module; }

    //! \returns The native script module object.
    auto native() const noexcept -> const asIScriptModule* { return _script_module; }

    //! Loads the given file into the module.
    void load_file(const std::filesystem::path& file) noexcept;

    //! Loads the given string of data into the module.
    void load_data(const std::string& name, const std::string& data) noexcept;

    //! Finishes the module build stage.
    void finalize() noexcept;

    //! Helper class to iterate over all types defined the module.
    class Types;

    //! \returns A type iterator object.
    auto types() const noexcept -> Types;

    //! \returns A script type of the given name if any is found.
    auto type_by_name(const std::string& name) const noexcept -> Type;

public:
    //! \returns The metadata string associated with the given type (if any is found).
    auto get_metadata(const Type& type) const noexcept -> std::string;

    //! \returns The metadata string associated with the given property (if any is found).
    auto get_metadata(const Property& prop) const noexcept -> std::string;

public:
    //! Finds the given module or creates a new one depending on the policy.
    //! \note By default tries first to find a given module then to create a new one.
    static auto get(Engine& engine, const std::string& name, Policy policy = Policy::CreateIfMissing) noexcept -> Module;

    //! Finds the given module or creates a new one depending on the policy.
    //! \note By default tries first to find a given module then to create a new one.
    static auto find(const Engine& engine, const std::string& name) noexcept -> Module;

private:
    asIScriptModule* _script_module;
    std::unique_ptr<ModuleLoader, void(*)(ModuleLoader*) noexcept> _module_loader;
};

} // namespace editor::script
//
//namespace Scripts
//{
//	namespace AngelScript
//	{
//		class AsScriptModule
//		{
//		public:
//			using ptr = std::shared_ptr<AsScriptModule>;
//
//			AsScriptModule(asIScriptEngine* engine, std::string name);
//			~AsScriptModule() = default;
//
//			asIScriptFunction* GetFunctionByName(const std::string& name) const;
//			asITypeInfo* GetTypeInfoByName(const std::string& name) const;
//
//			// Object types
//			asITypeInfo* GetObjectTypeByIndex(int index) const;
//			asITypeInfo* GetObjectTypeByIndex(int index, std::string& metadata) const;
//			int GetObjectTypeCount() const;
//
//            // Metadata values
//            std::string GetMetadataForType(asITypeInfo* typeinfo) const;
//            std::string GetMetadataForMethod(asITypeInfo* typeinfo, asIScriptFunction* function) const;
//            std::string GetMetadataForProperty(asITypeInfo* typeinfo, int index) const;
//
//			// Script handling
//			void AddSectionFromFile(const std::string& file);
//			void Build();
//
//        public:
//            std::string current_include_directory() const;
//            void push_include_directory(std::string);
//            void pop_include_directory();
//
//        private:
//            std::stack<std::string> m_IncludeStack;
//
//		private:
//			std::unique_ptr<CScriptBuilder> m_Builder;
//			asIScriptModule* m_Module;
//		};
//	}
//}

