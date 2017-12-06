#include "as_script_module.h"

#include <QDebug>
#include <scriptbuilder.h>
#include <cassert>
#include <filesystem>
#include <fstream>

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

