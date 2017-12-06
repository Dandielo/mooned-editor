#include "as_context_pool.h"

using namespace ::Scripts::AngelScript;

CContextDeleter::CContextDeleter(AsContextPool* pool) : m_Pool(pool) { }
CContextDeleter::CContextDeleter(const CContextDeleter& other) : m_Pool(other.m_Pool) { }
CContextDeleter::CContextDeleter(CContextDeleter&& other) : m_Pool(other.m_Pool) { }

void CContextDeleter::operator()(asIScriptContext* context) const {
	m_Pool->ReturnContext(context);
}

AsContextPool::AsContextPool(asIScriptEngine* engine) : m_ScriptEngine(engine)
{
	m_RequestFunc = [](asIScriptEngine* engine, AsContextPool* pool) -> asIScriptContext* {
		return pool->RequestContext();
	};
	m_ReturnFunc = [](asIScriptEngine* engine, asIScriptContext* context, AsContextPool* pool) {
		pool->ReturnContext(context);
	};

	// Register the created callbacks
	m_ScriptEngine->SetContextCallbacks(reinterpret_cast<asREQUESTCONTEXTFUNC_t>(m_RequestFunc), reinterpret_cast<asRETURNCONTEXTFUNC_t>(m_ReturnFunc), this);
}

AsContextPool::~AsContextPool()
{
	// Release all contexts
	for (auto ctx : m_ContextList)
		ctx->Release();
}

script_context_ptr AsContextPool::LocalContext()
{
	return script_context_ptr(RequestContext(), CContextDeleter(this));
}

asIScriptContext* AsContextPool::RequestContext()
{
	asIScriptContext* ctx;
	if (m_ContextList.empty())
	{
		ctx = m_ScriptEngine->CreateContext();
	}
	else
	{
		ctx = *m_ContextList.rbegin();
		m_ContextList.pop_back();
	}
	return ctx;
}

void AsContextPool::ReturnContext(asIScriptContext* context)
{
	m_ContextList.push_back(context);
	context->Unprepare();
}
