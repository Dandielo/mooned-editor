#pragma once
#include "CScriptObject.h"

#include <scripts/angelscript/as_context_pool.h>
#include <scripts/angelscript_new/method.h>

#define M_SCRIPT_TYPE(type, name) \
	friend class Scripts::CNativeScriptObject<##type>; \
	static constexpr auto ClassName = name "Native"; \
	static constexpr auto ScriptClassName = name; \
	static void registerTypeInterface(asIScriptEngine* engine)


namespace Scripts
{

using editor::script::ScriptObject;

template<class T>
class CNativeScriptObject : public editor::script::ScriptClass
{
public:
    static void registerType(asIScriptEngine* engine);
    static void registerTypeNoFactory(asIScriptEngine* engine);

protected:
    CNativeScriptObject(ScriptObject&& object);
    virtual ~CNativeScriptObject() override;

    void AddRef();
    void Release();

    static T* ObjectFactory();

    template<class... Args>
    void CallScriptMethod(const std::string& name, Args&&... args) const;

private:
    asILockableSharedBool* m_IsDead;
    int m_RefCount;

    friend class CScriptManager;
};

    // Include the inline file
#       include "CNativeScriptObject.inl"
}
