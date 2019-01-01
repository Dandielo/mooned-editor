#pragma once
#include <scripts/angelscript_new/object.h>

#include <string>
#include <unordered_map>

namespace editor::script
{
    //! A base class for creating an interface between a native and script class.
	class ScriptClass
	{
	public:
        ScriptClass(ScriptObject&& object);
        virtual ~ScriptClass() noexcept;

        //! \returns The script representation of the current object instance.
        auto script_object() noexcept -> ScriptObject& { return _script_object; }

        //! \returns The script representation of the current object instance.
        auto script_object() const noexcept -> const ScriptObject& { return _script_object; }

    protected:
        asIScriptFunction* GetScriptMethod(const char* name, bool cache = true) const;

        ScriptObject _script_object;
	};
}
