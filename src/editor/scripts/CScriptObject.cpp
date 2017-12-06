#include "CScriptObject.h"
#include <QDebug>

Scripts::CScriptObject::CScriptObject(asIScriptObject* obj) : _script_object(obj)
{
    _script_object->AddRef();
}

Scripts::CScriptObject::~CScriptObject()
{
    _script_object->Release();
}

asIScriptObject* Scripts::CScriptObject::ScriptObject() const
{
    return _script_object;
}

asIScriptFunction* Scripts::CScriptObject::GetScriptMethod(const char* name, bool cache /*= true*/)
{
    if (cache) qDebug("Caching is not yet implemented!");
    return _script_object->GetObjectType()->GetMethodByName(name);
}

void Scripts::CScriptObject::CallScriptMethod(const char* name)
{
    auto engine = _script_object->GetEngine();
    auto ctx = engine->RequestContext();

    auto func = GetScriptMethod(name, false);
    if (func) {
        ctx->Prepare(func);
        ctx->SetObject(_script_object);
        ctx->Execute();
    }

    engine->ReturnContext(ctx);
}

