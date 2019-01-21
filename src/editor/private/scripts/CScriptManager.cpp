#include "CScriptManager.h"

//#include "qt/QEditorWindow.h"
//#include "qt/workspace/QWorkspace.h"
//#include "qt/nodes/QNode.h"
//#include "qt/nodes/QNodePin.h"
//#include "qt/nodes/QNodeProperty.h"
//#include "qt/nodes/QNodePropertyApi.h"

#include "windows/scripted/QScriptedWorkspaceWindow.h"
#include "project/scripted/QScriptedProject.h"
#include "project/scripted/QScriptedProjectExporter.h"
#include "graph/scripted/QScriptedGraph.h"
#include "graph/scripted/QScriptedNode.h"
#include "graph/scripted/QScriptedNodeProperty.h"

#include <scripts/angelscript/as_script_module.h>
#include <scripts/angelscript_new/type.h>
#include <scripts/angelscript_new/property.h>

using namespace Scripts;

CScriptManager::CScriptManager()
    : _database{ }
{
    auto* script_engine = _engine.native();

    // Built-in types
    editor::QScriptedNodeProperty::registerType(script_engine);
    editor::QScriptedNode::registerType(script_engine);
    editor::QScriptedGraph::registerTypeNoFactory(script_engine);
    editor::QScriptedProject::registerTypeNoFactory(script_engine);
    editor::QScriptedProjectExporter::registerType(script_engine);
    QScriptedWorkspaceWindow::registerTypeNoFactory(script_engine);
}

void Scripts::CScriptManager::load(const std::filesystem::path& file) noexcept
{
    using editor::script::Module;

    auto script_module = _engine.get_module("engine", Module::Policy::CreateIfMissing);
    script_module.load_file(file);
    script_module.finalize();

    _database.extract_module(script_module);
}


auto Scripts::CScriptManager::find_type(const std::string& name) const noexcept -> editor::script::Type
{
    editor::script::Type result;
    auto types = _database.query_types(name);
    if (!types.empty())
    {
        result = types.front();
    }
    return result;
}

auto Scripts::CScriptManager::query_types(const std::string& query) const noexcept -> std::vector<editor::script::Type>
{
    return _database.query_types(query);
}

std::string CScriptManager::GetTypeAttr(asITypeInfo* type, std::string attribute) const
{
    return _database.get_attribute(editor::script::Type::from_native(type), attribute);
}

std::string CScriptManager::GetTypeAttr(asITypeInfo* type, std::string attribute, std::string default) const
{
    auto result = _database.get_attribute(editor::script::Type::from_native(type), attribute);
    if (result.empty()) result = default;
    return result;
}

std::string CScriptManager::GetPropertyAttr(asITypeInfo* type, int index, std::string attribute) const
{
    return _database.get_attribute(editor::script::Property::from_index(editor::script::Type::from_native(type), index), attribute);
}

std::string CScriptManager::GetPropertyAttr(asITypeInfo* type, int index, std::string attribute, std::string default) const
{
    auto result = _database.get_attribute(editor::script::Property::from_index(editor::script::Type::from_native(type), index), attribute);
    if (result.empty()) result = default;
    return result;
}

asITypeInfo* CScriptManager::GetTypeByName(const std::string& name) const
{
	return _engine.native()->GetTypeInfoByName(name.c_str());
}


