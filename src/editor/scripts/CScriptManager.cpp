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
    editor::QScriptedGraph::registerType(script_engine);
    editor::QScriptedProject::registerTypeNoFactory(script_engine);
    editor::QScriptedProjectExporter::registerType(script_engine);
    QScriptedWorkspaceWindow::registerType(script_engine);
}

void CScriptManager::Initialize(std::string init_file)
{
    using editor::script::Module;

    // Get the 'default' module.
    Module script_module = _engine.get_module("engine", Module::Policy::CreateIfMissing);
    script_module.load_file(init_file);
    script_module.finalize();

    // Extract type information from this module
    _database.extract_module(script_module);
}

editor::script::Type Scripts::CScriptManager::GetTypeInfo(const std::string& type) const noexcept
{
    editor::script::Type result;

    // Query for the given type
    auto types = _database.query_types(type);
    if (!types.empty())
    {
        // Only return the first result
        result = types.front();
    }

    return result;
}

std::vector<editor::script::Type> CScriptManager::QueryTypes(const std::string& query) const noexcept
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


