#include "CScriptObject.h"
#include <QDebug>

#include <scripts/angelscript_new/method.h>

//
//Scripts::CScriptObject::CScriptObject(asIScriptObject* obj) : _script_object(obj)
//{
//    _script_object->AddRef();
//}
//
//Scripts::CScriptObject::~CScriptObject()
//{
//    _script_object->Release();
//}
//
//asIScriptObject* Scripts::CScriptObject::ScriptObject() const
//{
//    return _script_object;
//}
//
//asIScriptFunction* Scripts::CScriptObject::GetScriptMethod(const char* name, bool cache /*= true*/) const
//{
//    if (cache) qDebug("Caching is not yet implemented!");
//    return _script_object->GetObjectType()->GetMethodByName(name);
//}
//
//void Scripts::CScriptObject::CallScriptMethod(const char* name)
//{
//    auto engine = _script_object->GetEngine();
//    auto ctx = engine->RequestContext();
//
//    auto func = GetScriptMethod(name, false);
//    if (func) {
//        ctx->Prepare(func);
//        ctx->SetObject(_script_object);
//        ctx->Execute();
//    }
//
//    engine->ReturnContext(ctx);
//}

namespace editor::script
{

ScriptClass::ScriptClass(ScriptObject&& object)
    : _script_object{ std::move(object) }
{ }

ScriptClass::~ScriptClass() noexcept
{

}

asIScriptFunction* ScriptClass::GetScriptMethod(const char* name, bool cache /*= true*/) const
{
    return Method::from_name(_script_object.type(), name).native();
}

}