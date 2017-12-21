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

using namespace Scripts;
//
//using Editor::EQt::QNode;
//using Editor::EQt::QNodeProperty;
//using Editor::EQt::QWorkspace;
//using Editor::EQt::QEditorWindow;

static inline void TypeInfoCleanupCallback(asITypeInfo* typeinfo)
{
	delete reinterpret_cast<SClassMetadata*>(typeinfo->GetUserData(1));
}

CScriptManager::CScriptManager() : _interpreter(nullptr)
{
    _interpreter = std::make_shared<::Scripts::AngelScript::AsInterpreter>();
    asIScriptEngine* engine = *_interpreter;
    engine->SetTypeInfoUserDataCleanupCallback(TypeInfoCleanupCallback, SClassMetadata::ClassType);

    // Built-in types
    editor::QScriptedNodeProperty::registerType(engine);
    editor::QScriptedNode::registerType(engine);
    editor::QScriptedGraph::registerType(engine);
    editor::QScriptedProject::registerType(engine);
    editor::QScriptedProjectExporter::registerType(engine);
    QScriptedWorkspaceWindow::registerType(engine);
    //Editor::EQt::asRegisterPropertyApi(engine);
    //QNode::RegisterType(engine);
    //QWorkspace::RegisterType(*m_Interpreter);
    //QEditorWindow::RegisterType(*m_Interpreter);
}

void CScriptManager::Initialize(std::string init_file)
{
	_interpreter->AddScript(init_file);
	_interpreter->RebuildScripts();

    // Get the default module and build the database
	auto module = _interpreter->GetDefaultModule();
    _database.AddModule(module);
}

std::vector<asITypeInfo*> CScriptManager::QueryTypes(std::string query) const
{
    return _database.QueryTypes(query);
}

std::string CScriptManager::GetTypeAttr(asITypeInfo* type, std::string attribute) const
{
    return _database.GetTypeAttr(type, attribute);
}

std::string CScriptManager::GetTypeAttr(asITypeInfo* type, std::string attribute, std::string default) const
{
    auto result = _database.GetTypeAttr(type, attribute);
    if (result.empty()) result = default;
    return result;
}

std::string CScriptManager::GetPropertyAttr(asITypeInfo* type, int index, std::string attribute) const
{
    return _database.GetPropertyAttr(type, index, attribute);
}

std::string CScriptManager::GetPropertyAttr(asITypeInfo* type, int index, std::string attribute, std::string default) const
{
    auto result = _database.GetPropertyAttr(type, index, attribute);
    if (result.empty()) result = default;
    return result;
}

asITypeInfo* CScriptManager::GetTypeByName(const std::string& name) const
{
	asIScriptEngine* engine = *_interpreter;
	return engine->GetTypeInfoByName(name.c_str());
}


