#include <scripts/angelscript/as_script_module.h>
#include <scripts/angelscript/as_interpreter.h>
#include <scripts/angelscript_new/base.h>
#include <scripts/angelscript_new/property.h>

#include "scripts/angelscript_new/private.h"

#include <scriptbuilder.h>

#include <QDebug>
#include <cassert>
#include <filesystem>
#include <fstream>

namespace editor::script
{

class ModuleLoader
{
public:
    ModuleLoader() noexcept = default;
    virtual ~ModuleLoader() noexcept = default;

    //! Creates a new module with the given name.
    virtual auto start_new_module(const std::string& name) noexcept -> asIScriptModule* = 0;

    //! Adds the file as a script section into the module.
    virtual void add_section_from_file(const std::filesystem::path& file) noexcept = 0;

    //! Adds some data as a script section into the module.
    virtual void add_section_from_memory(const std::string& name, const std::string& data) noexcept = 0;

    //! Finishes the module creation.
    virtual void finalize_module() noexcept = 0;

    //! The pointer type
    using ptr_t = std::unique_ptr<ModuleLoader, void(*)(ModuleLoader*) noexcept>;

    virtual auto to_passive_loader() noexcept -> ptr_t = 0;

public:
    //! \returns The metadata string for the given Type if any is found in the module loader.
    virtual auto get_metadata(const Type& type) const noexcept -> std::string = 0;

    //! \returns The metadata string for the given Property if any is found in the module loader.
    virtual auto get_metadata(const Property& prop) const noexcept -> std::string = 0;

public:
    //! Module loader deleter.
    static void deleter_function(ModuleLoader* loader) noexcept
    {
        delete loader;
    }
};

//////////////////////////////////////////////////////////////////////////

class ActiveModuleLoader;

class PassiveModuleLoader : public ModuleLoader
{
public:
    PassiveModuleLoader(std::unique_ptr<CScriptBuilder> builder)
        : _script_builder{ std::move(builder) }
    { }

    virtual ~PassiveModuleLoader() noexcept = default;

    auto start_new_module(const std::string& name) noexcept -> asIScriptModule* override { return _script_builder->GetModule(); }

    void add_section_from_file(const std::filesystem::path& file) noexcept { }

    void add_section_from_memory(const std::string& name, const std::string& data) noexcept { }

    void finalize_module() noexcept override { }

    auto to_passive_loader() noexcept -> ptr_t override
    {
        return ptr_t{ new PassiveModuleLoader{ std::move(_script_builder) }, &deleter_function };
    }

public:
    //! \returns The metadata string for the given Type if any is found in the module loader.
    auto get_metadata(const Type& type) const noexcept -> std::string override
    {
        std::string result;
        if (_script_builder)
        {
            result = _script_builder->GetMetadataStringForType(type.native()->GetTypeId());
        }
        return result;
    }

    //! \returns The metadata string for the given Property if any is found in the module loader.
    auto get_metadata(const Property& prop) const noexcept -> std::string override
    {
        std::string result;
        if (_script_builder)
        {
            result = _script_builder->GetMetadataStringForTypeProperty(prop.owning_type().native()->GetTypeId(), prop.index());
        }
        return result;
    }

private:
    std::unique_ptr<CScriptBuilder> _script_builder;
};

//////////////////////////////////////////////////////////////////////////

class ActiveModuleLoader : public ModuleLoader
{
public:
    ActiveModuleLoader(Engine& engine) noexcept
        : ModuleLoader{ }
        , _script_engine{ engine }
        , _script_builder{ std::make_unique<CScriptBuilder>() }
    {
        _script_builder->SetIncludeCallback(reinterpret_cast<INCLUDECALLBACK_t>(&angelscript_include_callback), this);
    }

    ~ActiveModuleLoader() noexcept override = default;

    //! Creates a new module with the given name.
    auto start_new_module(const std::string& name) noexcept -> asIScriptModule* override
    {
        _script_builder->StartNewModule(_script_engine.native(), name.c_str());
        return _script_builder->GetModule();
    }

    //! Adds the file as a script section into the module.
    void add_section_from_file(const std::filesystem::path& file) noexcept override
    {
        _script_builder->AddSectionFromFile(file.generic_string().c_str());
    }

    //! Adds some data as a script section into the module.
    void add_section_from_memory(const std::string& name, const std::string& data) noexcept override
    {
        _script_builder->AddSectionFromMemory(name.c_str(), data.c_str());
    }

    //! Builds the module.
    void finalize_module() noexcept override
    {
        [[maybe_unused]] bool result = _script_builder->BuildModule() == asSUCCESS;
        assert(result);
    }

    //! Creates a passive loader from an active loader.
    auto to_passive_loader() noexcept -> ptr_t override
    {
        return ptr_t{ new PassiveModuleLoader{ std::move(_script_builder) }, &deleter_function };
    }

public:
    //! \returns The metadata string for the given Type if any is found in the module loader.
    auto get_metadata(const Type& type) const noexcept -> std::string override
    {
        std::string result;
        if (_script_builder)
        {
            result = _script_builder->GetMetadataStringForType(type.id().value);
        }
        return result;
    }

    //! \returns The metadata string for the given Property if any is found in the module loader.
    auto get_metadata(const Property& prop) const noexcept -> std::string override
    {
        std::string result;
        if (_script_builder)
        {
            result = _script_builder->GetMetadataStringForTypeProperty(prop.owning_type().id().value, prop.index());
        }
        return result;
    }

public:
    //! Resolves the given include given the source file.
    auto resolve_include(const std::filesystem::path& include, const std::filesystem::path& source_file) const noexcept -> std::filesystem::path
    {
        namespace fs = std::filesystem;

        if (include.is_absolute())
        {
            return include;
        }

        // Check against the current source file...
        fs::path result = source_file.parent_path() / include;
        if (fs::is_regular_file(result))
        {
            return result;
        }

        // Check against the current working directory...
        result = _current_working_dir / include;
        if (fs::is_regular_file(result))
        {
            return result;
        }

        return{ };
    }

protected:
    //! Callback function for #include statements in AngelScript
    static void angelscript_include_callback(const char* include, const char* source_file, CScriptBuilder* script_builder, ActiveModuleLoader* script_loader) noexcept;

private:
    Engine& _script_engine;
    std::unique_ptr<CScriptBuilder> _script_builder;

    // Get the current working directory
    const std::filesystem::path _current_working_dir{ std::filesystem::current_path() };

    // Friends
    friend class PassiveModuleLoader;
};

//////////////////////////////////////////////////////////////////////////

void ActiveModuleLoader::angelscript_include_callback(const char* include, const char* source_file, CScriptBuilder* script_builder, ActiveModuleLoader* script_loader) noexcept
{
    namespace fs = std::filesystem;

    fs::path include_path = script_loader->resolve_include(include, source_file);

    if (!fs::is_regular_file(include_path))
    {
        qDebug("[%s] Include file not found: '%s', from: '%s'", "AngelScript::Module", include_path.generic_string().c_str(), source_file);
        std::exit(1);
        return;
    }

    qDebug("[%s] Included: '%s', from: '%s'", "AngelScript::Module", include_path.generic_string().c_str(), source_file);
    script_loader->add_section_from_file(include_path);
}

//////////////////////////////////////////////////////////////////////////

Module::Module(ConstructorData<Module> data) noexcept
    : _script_module{ data.module }
    , _module_loader{ std::move(data.loader) }
{ }

void Module::load_file(const std::filesystem::path& file) noexcept
{
    _module_loader->add_section_from_file(file);
}

void Module::load_data(const std::string& name, const std::string& data) noexcept
{
    _module_loader->add_section_from_memory(name, data);
}

void Module::finalize() noexcept
{
    _module_loader->finalize_module();
    _module_loader = _module_loader->to_passive_loader();
}

auto Module::types() const noexcept -> Types
{
    return Types{ _script_module, _script_module->GetObjectTypeCount() };
}

auto Module::type_by_name(const std::string& name) const noexcept -> Type
{
    Type result;
    for (const auto& type : types())
    {
        if (type.name() == name)
        {
            result = type;
            break;
        }
    }
    return result;
}

auto Module::get_metadata(const Type& type) const noexcept -> std::string
{
    return _module_loader->get_metadata(type);
}

auto Module::get_metadata(const Property& prop) const noexcept -> std::string
{
    return _module_loader->get_metadata(prop);
}

//////////////////////////////////////////////////////////////////////////

auto Module::get(Engine& engine, const std::string& name, Policy policy /*= Policy::CreateIfMissing*/) noexcept -> Module
{
    asIScriptModule* script_module = nullptr;
    ModuleLoader::ptr_t module_loader = ModuleLoader::ptr_t{ new PassiveModuleLoader{ nullptr }, &ModuleLoader::deleter_function };

    if (policy != Policy::AlwaysCreate)
    {
        script_module = engine.native()->GetModule(name.c_str(), asGM_ONLY_IF_EXISTS);

        if (script_module == nullptr && policy == Policy::CreateIfMissing)
        {
            policy = Policy::AlwaysCreate;
        }
    }

    if (policy == Policy::AlwaysCreate)
    {
        module_loader = ModuleLoader::ptr_t{ new ActiveModuleLoader{ engine }, &ModuleLoader::deleter_function };
        script_module = module_loader->start_new_module(name);
    }

    return ConstructorData<Module>{ script_module, std::move(module_loader) };
}

auto Module::find(const Engine& engine, const std::string& name) noexcept -> Module
{
    asIScriptModule* script_module = const_cast<asIScriptEngine*>(engine.native())->GetModule(name.c_str(), asGM_ONLY_IF_EXISTS);
    return ConstructorData<Module>{ script_module, std::unique_ptr<ModuleLoader, void(*)(ModuleLoader*) noexcept>(new PassiveModuleLoader{ nullptr }, &ModuleLoader::deleter_function) };
}

} // namespace editor::script

#if 0

using namespace ::Scripts::AngelScript;
namespace stdfs = std::experimental::filesystem::v1;

static inline void AddSectionFromFileHelper(CScriptBuilder* builder, const std::string& filename, std::ifstream& script, size_t size)
{
	char* buffer = new char[size + 1];
	script.read(buffer, sizeof(char) * size);
	buffer[size] = 0;


	int result = builder->AddSectionFromMemory(filename.c_str(), buffer, static_cast<unsigned>(size));
	assert(result >= 0);

	delete[] buffer;
}

static inline int IncludeCallback(const char* include, const char* from, CScriptBuilder* builder, AsScriptModule* userdata)
{
    stdfs::path cid = userdata->current_include_directory();
    stdfs::path path = cid / include;

	int result = 0;
	if (stdfs::is_regular_file(path))
	{
        qDebug("[%s] Included: '%s', from: '%s'", "AngelScript::Module", include, from);

        auto base_path = stdfs::path(include).parent_path();
        userdata->push_include_directory((cid / base_path).generic_string());

        std::ifstream include_file{ path, std::ios::binary | std::ios::in };
		AddSectionFromFileHelper(builder, include, include_file, stdfs::file_size(path));
        userdata->pop_include_directory();
	}
	else
	{
        qDebug("[%s] Include file not found: '%s', from: '%s'", "AngelScript::Module", path.c_str(), from);
		result = -1;
	}
	return result;
}

AsScriptModule::AsScriptModule(asIScriptEngine* engine, std::string name)
{
	m_Builder = std::make_unique<CScriptBuilder>();
	m_Builder->SetIncludeCallback(reinterpret_cast<INCLUDECALLBACK_t>(IncludeCallback), this);
	m_Builder->StartNewModule(engine, name.c_str());
	m_Module = engine->GetModule(name.c_str());

    m_IncludeStack.push(stdfs::current_path().generic_string());
}

asIScriptFunction* AsScriptModule::GetFunctionByName(const std::string& name) const
{
	return m_Module->GetFunctionByName(name.c_str());
}

asITypeInfo* AsScriptModule::GetTypeInfoByName(const std::string& name) const
{
	return m_Module->GetTypeInfoByName(name.c_str());
}

asITypeInfo* AsScriptModule::GetObjectTypeByIndex(int index) const
{
	return m_Module->GetObjectTypeByIndex(index);
}

asITypeInfo* AsScriptModule::GetObjectTypeByIndex(int index, std::string& metadata) const
{
	auto typeinfo = GetObjectTypeByIndex(index);
	metadata = GetMetadataForType(typeinfo);
	return typeinfo;
}

std::string AsScriptModule::GetMetadataForType(asITypeInfo* typeinfo) const
{
    return m_Builder->GetMetadataStringForType(typeinfo->GetTypeId());;
}

std::string AsScriptModule::GetMetadataForMethod(asITypeInfo* typeinfo, asIScriptFunction* function) const
{
    return m_Builder->GetMetadataStringForTypeMethod(typeinfo->GetTypeId(), function);
}

std::string AsScriptModule::GetMetadataForProperty(asITypeInfo* typeinfo, int index) const
{
    return m_Builder->GetMetadataStringForTypeProperty(typeinfo->GetTypeId(), index);
}

int AsScriptModule::GetObjectTypeCount() const
{
	return m_Module->GetObjectTypeCount();
}

void AsScriptModule::AddSectionFromFile(const std::string& file)
{
    if (stdfs::is_regular_file(file))
	{
        auto base_path = stdfs::path(file).parent_path();
        push_include_directory(base_path.generic_string());

        std::ifstream in_stream{ file, std::ios::binary | std::ios::in };

		AddSectionFromFileHelper(m_Builder.get(), file.c_str(), in_stream, stdfs::file_size(file));
        pop_include_directory();
	}
}

void AsScriptModule::Build()
{
	m_Builder->BuildModule();
}

std::string Scripts::AngelScript::AsScriptModule::current_include_directory() const
{
    return m_IncludeStack.top();
}

void Scripts::AngelScript::AsScriptModule::push_include_directory(std::string path)
{
    m_IncludeStack.push(path);
}

void Scripts::AngelScript::AsScriptModule::pop_include_directory()
{
    m_IncludeStack.pop();
}

#endif
