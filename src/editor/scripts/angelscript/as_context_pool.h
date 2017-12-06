#pragma once
#include <vector>
#include <memory>
#include <angelscript.h>

namespace Scripts
{
    namespace AngelScript
    {
		class AsContextPool;
		class CContextDeleter;

		using script_context_ptr = std::unique_ptr<asIScriptContext, CContextDeleter>;

		class CContextDeleter
		{
		public:
			CContextDeleter(AsContextPool* pool);
			CContextDeleter(const CContextDeleter& other);
			CContextDeleter(CContextDeleter&& other);
			~CContextDeleter() = default;

			void operator()(asIScriptContext* context) const;

		private:
			AsContextPool* m_Pool;
		};

		// The context pool
		class AsContextPool
		{
		public:
			using ptr = std::unique_ptr<AsContextPool>;
			using context_vector = std::vector<asIScriptContext*>;
			using return_func_t = void(*)(asIScriptEngine*, asIScriptContext*, AsContextPool*);
			using request_func_t = asIScriptContext*(*)(asIScriptEngine*, AsContextPool*);

			AsContextPool(asIScriptEngine* engine);
			~AsContextPool();

			script_context_ptr LocalContext();
			asIScriptContext* RequestContext();
			void ReturnContext(asIScriptContext* context);

		private:
			asIScriptEngine* m_ScriptEngine;
			context_vector m_ContextList;

			request_func_t m_RequestFunc;
			return_func_t m_ReturnFunc;
		};
	}
}
