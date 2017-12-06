#pragma once
#include "CScriptObject.h"

#define M_SCRIPT_TYPE(type, name) \
	friend class Scripts::CNativeScriptObject<##type>; \
	static constexpr auto ClassName = name "Native"; \
	static constexpr auto ScriptClassName = name; \
	static void registerTypeInterface(asIScriptEngine* engine)

namespace Scripts
{
    template<class T>
    class CNativeScriptObject : public CScriptObject
    {
    public:
        static void registerType(asIScriptEngine* engine);

    protected:
        CNativeScriptObject(asIScriptObject* object);
        virtual ~CNativeScriptObject() override;

        void AddRef();
        void Release();

        static T* ObjectFactory();

        template<class... Args>
        void CallScriptMethod(const char* name, Args&&... args);

    private:
        asILockableSharedBool* m_IsDead;
        int m_RefCount;

        friend class CScriptManager;
    };

    // Include the inline file
#       include "CNativeScriptObject.inl"
}
