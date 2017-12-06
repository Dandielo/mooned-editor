#pragma once
#include <string>
#include <unordered_map>
#include "angelscript/as_interpreter.h"

namespace Scripts
{
	class CScriptObject
	{
	public:
        CScriptObject(asIScriptObject* obj);
        virtual ~CScriptObject();

        // Methods
        asIScriptObject* ScriptObject() const;

    protected:
        asIScriptFunction* GetScriptMethod(const char* name, bool cache = true);
        void CallScriptMethod(const char* name);

        asIScriptObject* _script_object;
	};
}
