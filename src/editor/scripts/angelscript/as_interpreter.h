#pragma once
#include <unordered_map>
#include <angelscript.h>

#include "as_script_module.h"
#include "as_context_pool.h"

namespace Scripts
{
	namespace AngelScript
	{
		class AsInterpreter
		{
		public:
			using ptr = std::shared_ptr<AsInterpreter>;
			static asIScriptContext* GetActiveContext();

			AsInterpreter();
			~AsInterpreter();

			script_context_ptr GetContext();
			AsScriptModule::ptr GetDefaultModule();

			asIScriptObject* CreateScriptObject(const std::string& name);
				
			// User data
			void* GetUserdata(size_t size) const;

			// Script handling 
			void AddScript(const std::string& path);
			void RebuildScripts();

			// Operators
			operator asIScriptEngine*() const { return m_Engine; }

		protected:
			asIScriptEngine* m_Engine;
			AsContextPool::ptr m_ContextPool;
			std::unordered_map<std::string, AsScriptModule::ptr> m_Modules;
		};
	}
}
