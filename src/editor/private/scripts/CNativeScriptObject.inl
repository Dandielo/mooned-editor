template<class T>
Scripts::CNativeScriptObject<T>::CNativeScriptObject(ScriptObject&& object)
    : ScriptClass{ std::move(object) }
    , m_RefCount{ 1 }
    , m_IsDead{ nullptr }
{
    m_IsDead = script_object().native()->GetWeakRefFlag();
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
void Scripts::CNativeScriptObject<T>::registerTypeNoFactory(asIScriptEngine* engine)
{
    int r;
    std::string factory_name = std::string(T::ClassName) + "@ f()";

    r = engine->RegisterObjectType(T::ClassName, 0, asOBJ_REF | asOBJ_NOCOUNT);

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
    auto ctx = asGetActiveContext();
    auto udata = reinterpret_cast<uintptr_t>(ctx->GetUserData(0x01));

    if (udata != 1)
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
    return new T{ editor::script::ScriptObject::from_native(obj) };
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
    ctx->SetArgWord(index, value);
}

template<>
inline void SetContextArgument<asDWORD>(asIScriptContext* ctx, asUINT index, asDWORD value) {
    ctx->SetArgDWord(index, value);
}

template<>
inline void SetContextArgument<asQWORD>(asIScriptContext* ctx, asUINT index, asQWORD value) {
    ctx->SetArgQWord(index, value);
}

template<>
inline void SetContextArgument<float>(asIScriptContext* ctx, asUINT index, float value) {
    ctx->SetArgFloat(index, value);
}

template<>
inline void SetContextArgument<double>(asIScriptContext* ctx, asUINT index, double value) {
    ctx->SetArgDouble(index, value);
}

template<class T>
template<class... Args>
void Scripts::CNativeScriptObject<T>::CallScriptMethod(const std::string& name, Args&&... args) const
{
    auto wrapper = [](...) {};

    /*
        auto engine = _script_object.native()->GetEngine();
        auto ctx = engine->RequestContext();*/

    auto method = editor::script::Method::from_name(_script_object.type(), name);
    if (method.valid())
    {
        auto context = editor::script::ScriptContext::from_any(_script_object);

        context.prepare(method, _script_object);
        context.call(std::forward<Args>(args)...);
    }

    //int index = 0;
    //auto func = GetScriptMethod(name, false);
    //if (func) {
    //    ctx->Prepare(func);
    //    ctx->SetObject(_script_object);

    //    wrapper((SetContextArgument(ctx, index++, args), 0)...);

    //    ctx->Execute();
    //}

    //engine->ReturnContext(ctx);
}
