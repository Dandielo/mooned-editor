#include <scripts/angelscript/as_context_pool.h>

#include <scripts/angelscript_new/function.h>
#include <scripts/angelscript_new/method.h>
#include <scripts/angelscript_new/object.h>

#include "scripts/angelscript_new/private.h"

#include <cassert>

namespace editor::script
{
namespace detail
{

//! Script context release function
void release_script_context(asIScriptContext* context) noexcept
{
    context->GetEngine()->ReturnContext(context);
}

} // namespace detail

editor::script::ScriptContext::ScriptContext(ConstructorData<ScriptContext> data) noexcept
    : _script_context{ data.context, &detail::release_script_context }
{
    _cleanup = []() {}; // First function is empty
}

ScriptContext::~ScriptContext() noexcept
{
    // Calling the cleanup function.
    _cleanup();
}

void ScriptContext::prepare(const Function& func) noexcept
{
    _script_context->Prepare(const_cast<asIScriptFunction*>(func.native()));
}

void ScriptContext::prepare(const Method& method, const ScriptObject& object) noexcept
{
    _script_context->Prepare(const_cast<asIScriptFunction*>(method.native()));
    _script_context->SetObject(const_cast<asIScriptObject*>(object.native()));
}

void ScriptContext::set_userdata(const void* userdata, uintptr_t type /*= 0*/) noexcept
{
    _script_context->SetUserData(const_cast<void*>(userdata), type);

    // Chain a new cleanup function
    // \note We copy the last function object so we can chain call them.
    _cleanup = [this, last_cleanup = std::move(_cleanup), type]()
    {
        last_cleanup();
        _script_context->SetUserData(0, type);
    };
}

auto ScriptContext::userdata(uintptr_t type /*= 0*/) const noexcept -> const void*
{
    return _script_context->GetUserData(type);
}

auto ScriptContext::from_engine(asIScriptEngine* engine) noexcept -> ScriptContext
{
    return ConstructorData<ScriptContext>{ engine->RequestContext() };
}

//////////////////////////////////////////////////////////////////////////

void ScriptContext::set_argument(asUINT index, asBYTE arg) noexcept
{
    _script_context->SetArgByte(index, arg);
}

void ScriptContext::set_argument(asUINT index, asWORD arg) noexcept
{
    _script_context->SetArgWord(index, arg);
}

void ScriptContext::set_argument(asUINT index, asDWORD arg) noexcept
{
    _script_context->SetArgDWord(index, arg);
}

void ScriptContext::set_argument(asUINT index, asQWORD arg) noexcept
{
    _script_context->SetArgQWord(index, arg);
}

void ScriptContext::set_argument(asUINT index, float arg) noexcept
{
    _script_context->SetArgFloat(index, arg);
}

void ScriptContext::set_argument(asUINT index, double arg) noexcept
{
    _script_context->SetArgDouble(index, arg);
}

void ScriptContext::set_argument(asUINT index, Object& object) noexcept
{
    _script_context->SetArgObject(index, object.address());
}

void ScriptContext::set_argument(asUINT index, ScriptObject& object) noexcept
{
    _script_context->SetArgObject(index, object.native());
}

void ScriptContext::set_argument(asUINT index, void* address) noexcept
{
    _script_context->SetArgAddress(index, address);
}

void ScriptContext::set_argument(asUINT index, const char* string) noexcept
{
    set_argument(index, std::string{ string });
}

void ScriptContext::execute() noexcept
{
    [[maybe_unused]] int result = _script_context->Execute();
    assert(result == 0);
}

} // namespace editor::script

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
