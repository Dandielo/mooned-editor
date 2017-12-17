#include "as_interpreter.h"

#include <scriptarray.h>
#include <scriptstdstring.h>

#include <QDebug>

using namespace ::Scripts::AngelScript;

// Implement a simple message callback function
static inline void MessageCallback(const asSMessageInfo *msg, void *param)
{
    using logfunc_t = void(*)(const char* msg, ...);

    if (msg->type == asMSGTYPE_WARNING)
    {
        qDebug("[%s] %s (%d, %d): %s", "AngelScript::Compiler", msg->section, msg->row, msg->col, msg->message);
    }
    else if (msg->type == asMSGTYPE_INFORMATION)
    {
        qDebug("[%s] %s (%d, %d): %s", "AngelScript::Compiler", msg->section, msg->row, msg->col, msg->message);
    }
    else
    {
        qDebug("[%s] %s (%d, %d): %s", "AngelScript::Compiler", msg->section, msg->row, msg->col, msg->message);
    }
}

static inline void PrintCallback(std::string message)
{
	auto ctx = asGetActiveContext();
    std::string location = "AngelScript || ";
    if (ctx->GetThisPointer(0))
        location += reinterpret_cast<asIScriptObject*>(ctx->GetThisPointer())->GetObjectType()->GetName() + std::string("::");
	location += ctx->GetFunction(0)->GetName();

    qDebug("[%s] %s", location.c_str(), message.c_str());
}

static inline void TypeInfoCleanupCallback(asITypeInfo* typeinfo)
{
	delete typeinfo->GetUserData(0);
}

// Implement the global alloc and free functions for the AS script engine
static inline void* AngelScriptAllocFunc(size_t size) {
    return malloc(size);
}

static inline void AngelScriptFreeFunc(void* ptr) {
    free(ptr);
}

asIScriptContext* AsInterpreter::GetActiveContext()
{
	return asGetActiveContext();
}

AsInterpreter::AsInterpreter() : m_Engine(nullptr), m_ContextPool(nullptr) {
	asSetGlobalMemoryFunctions(AngelScriptAllocFunc, AngelScriptFreeFunc);

	m_Engine = asCreateScriptEngine();
	m_ContextPool = std::make_unique<AsContextPool>(m_Engine);

	// Set the message callback
	m_Engine->SetMessageCallback(asFUNCTION(MessageCallback), nullptr, asCALL_CDECL);
	m_Engine->SetTypeInfoUserDataCleanupCallback(TypeInfoCleanupCallback);

	// Register the string type
	RegisterStdString(m_Engine);
    RegisterScriptArray(m_Engine, true);

	// Register a basic print function
	m_Engine->RegisterGlobalFunction("void print(string& in)", asFUNCTION(PrintCallback), asCALL_CDECL);

	// Create the default module
	auto module = std::make_shared<AsScriptModule>(m_Engine, std::string("engine"));
	m_Modules["engine"] = module;
}

script_context_ptr AsInterpreter::GetContext()
{
	return m_ContextPool->LocalContext();
}

AsScriptModule::ptr AsInterpreter::GetDefaultModule()
{
	return m_Modules["engine"];
}

asIScriptObject* AsInterpreter::CreateScriptObject(const std::string& name)
{
	asITypeInfo* type_info = GetDefaultModule()->GetTypeInfoByName(name.c_str());
	asIScriptObject* obj = reinterpret_cast<asIScriptObject*>(m_Engine->CreateScriptObject(type_info));
	return obj;
}

void* AsInterpreter::GetUserdata(size_t size) const
{
    return malloc(size);
}

void AsInterpreter::AddScript(const std::string& path)
{
	m_Modules["engine"]->AddSectionFromFile(path);
}

void AsInterpreter::RebuildScripts()
{
	m_Modules["engine"]->Build();
}

AsInterpreter::~AsInterpreter() {
	m_Engine->ShutDownAndRelease();
}
