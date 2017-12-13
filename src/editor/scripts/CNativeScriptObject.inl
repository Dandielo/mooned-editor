template<class T>
Scripts::CNativeScriptObject<T>::CNativeScriptObject(asIScriptObject* object) : CScriptObject(object), m_RefCount(1), m_IsDead(nullptr)
{
    m_IsDead = object->GetWeakRefFlag();
    m_IsDead->AddRef();
}

template<class T>
Scripts::CNativeScriptObject<T>::~CNativeScriptObject()
{
    m_IsDead->Release();
    m_IsDead = nullptr;
}

template<class T>
void Scripts::CNativeScriptObject<T>::registerType(asIScriptEngine* engine)
{
    int r;
    std::string factory_name = std::string(T::ClassName) + "@ f()";

    r = engine->RegisterObjectType(T::ClassName, 0, asOBJ_REF | asOBJ_NOCOUNT);
    r = engine->RegisterObjectBehaviour(T::ClassName, asBEHAVE_FACTORY, factory_name.c_str(), asFUNCTION(CNativeScriptObject::ObjectFactory), asCALL_CDECL);
    //r = engine->RegisterObjectBehaviour(T::ClassName, asBEHAVE_ADDREF, "void f()", asMETHOD(T, AddRef), asCALL_THISCALL);
    //r = engine->RegisterObjectBehaviour(T::ClassName, asBEHAVE_RELEASE, "void f()", asMETHOD(T, Release), asCALL_THISCALL);
    T::registerTypeInterface(engine);
}

template<class T>
void Scripts::CNativeScriptObject<T>::AddRef()
{
    m_RefCount++;

    if (!m_IsDead->Get())
        _script_object->AddRef();
}

template<class T>
void Scripts::CNativeScriptObject<T>::Release()
{
    if (m_IsDead && !m_IsDead->Get())
        _script_object->Release();

    --m_RefCount;
    //if (--m_RefCount == 0); // assert(m_IsDead == nullptr);
}

template<class T>
T* Scripts::CNativeScriptObject<T>::ObjectFactory()
{
    auto ctx = AsInterpreter::GetActiveContext();
    auto udata = reinterpret_cast<int*>(ctx->GetUserData(0x01));

    if (udata == nullptr || *udata != 1)
    {
        ctx->SetException("This object can be created only by the editor!");
        return nullptr;
    }

    auto func = ctx->GetFunction(0);
    if (func->GetObjectType() == 0 || strcmp(func->GetObjectType()->GetName(), T::ScriptClassName) != 0)
    {
        std::string msg = "Invalid attempt to manually instantiate ";
        msg += T::ClassName;
        ctx->SetException(msg.c_str());
        return nullptr;
    }

    auto obj = reinterpret_cast<asIScriptObject*>(ctx->GetThisPointer(0));
    return new T(obj);
}

template<class T>
void SetContextArgument(asIScriptContext* ctx, asUINT index, T* value)
{
    ctx->SetArgObject(index, value);
}

template<class T>
void SetContextArgument(asIScriptContext* ctx, asUINT index, T value) {
    //CHKIRD(FailAlways, "Undefined script type mapping!");
}

template<>
inline void SetContextArgument<asBYTE>(asIScriptContext* ctx, asUINT index, asBYTE value) {
    ctx->SetArgByte(index, value);
}

template<>
inline void SetContextArgument<asWORD>(asIScriptContext* ctx, asUINT index, asWORD value) {
    ctx->SetArgByte(index, value);
}

template<>
inline void SetContextArgument<asDWORD>(asIScriptContext* ctx, asUINT index, asDWORD value) {
    ctx->SetArgByte(index, value);
}

template<>
inline void SetContextArgument<asQWORD>(asIScriptContext* ctx, asUINT index, asQWORD value) {
    ctx->SetArgByte(index, value);
}

template<>
inline void SetContextArgument<float>(asIScriptContext* ctx, asUINT index, float value) {
    ctx->SetArgByte(index, value);
}

template<>
inline void SetContextArgument<double>(asIScriptContext* ctx, asUINT index, double value) {
    ctx->SetArgByte(index, value);
}

template<class T>
template<class... Args>
void Scripts::CNativeScriptObject<T>::CallScriptMethod(const char* name, Args&&... args)
{
    auto wrapper = [](...) {};
    auto engine = _script_object->GetEngine();
    auto ctx = engine->RequestContext();

    int index = 0;
    auto func = GetScriptMethod(name, false);
    if (func) {
        ctx->Prepare(func);
        ctx->SetObject(_script_object);

        wrapper((SetContextArgument(ctx, index++, args), 0)...);

        ctx->Execute();
    }

    engine->ReturnContext(ctx);
}
