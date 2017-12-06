#include "QScriptedNodeProperty.h"
#include "QScriptedNodePin.h"
#include "QScriptedNodePinValue.h"

#include "scripts/CScriptManager.h"

#include <QVariant>
#include <assert.h>

// Arithmetic operators
static void asPropertyGeneric_OpAdd(asIScriptGeneric* gen);
static void asPropertyGeneric_OpSub(asIScriptGeneric* gen);             // int operator-(const int&)
static void asPropertyGeneric_OpMul(asIScriptGeneric* gen);             // int operator*(const int&)
static void asPropertyGeneric_OpDiv(asIScriptGeneric* gen);             // int operator/(const int&)
static void asPropertyGeneric_OpMod(asIScriptGeneric* gen);             // int operator%(const int&)
static void asPropertyGeneric_OpPow(asIScriptGeneric* gen);             // int operator**(const int&)

                                                                        //////////////////////////////////////////////////////////////////////////
                                                                        // Type registration
static void registerGenericPropertyType(asIScriptEngine*);

static int QScriptedNodePropertyTypeSize = sizeof(editor::QScriptedNodeProperty);
static int QScriptedNodePropertyTypeTraits = asOBJ_VALUE | asGetTypeTraits<editor::QScriptedNodeProperty>();

template<class T>
void registerNativePropertyType(asIScriptEngine* engine, std::string name);

void editor::QScriptedNodeProperty::registerType(asIScriptEngine* engine)
{
    registerGenericPropertyType(engine);
    registerNativePropertyType<bool>(engine, "bool");
    registerNativePropertyType<int8_t>(engine, "int8");
    registerNativePropertyType<int16_t>(engine, "int16");
    registerNativePropertyType<int32_t>(engine, "int");
    registerNativePropertyType<int64_t>(engine, "int64");
    registerNativePropertyType<uint8_t>(engine, "uint8");
    registerNativePropertyType<uint16_t>(engine, "uint16");
    registerNativePropertyType<uint32_t>(engine, "uint");
    registerNativePropertyType<uint64_t>(engine, "uint64");
    registerNativePropertyType<float>(engine, "float");
    registerNativePropertyType<double>(engine, "double");
}

//////////////////////////////////////////////////////////////////////////
// Generic property type (is used as a base for all specializations and for reference types)
static bool asGenericPropertyTypeCheck(asITypeInfo* typeinfo, bool& dont_gc)
{
    // This template will only support primitive (for now)
    int type_id = typeinfo->GetSubTypeId();
    if (type_id & asTYPEID_MASK_OBJECT)
    {
        // We can allow user type handles to be stored in a property
        if (type_id & asTYPEID_OBJHANDLE)
        {
            return true;
        }

        // We cannot allow user types to be provided to a property
        return false;
    }

    // Don't garbage collect
    dont_gc = true;
    return true;
}

static void asPropertyGeneric_Construct(asITypeInfo* typeinfo, void* memory)
{
    new (memory) editor::QScriptedNodeProperty(typeinfo, typeinfo->GetSubTypeId());
}

static void asPropertyGeneric_Destruct(void* memory)
{
    reinterpret_cast<editor::QScriptedNodeProperty*>(memory)->~QScriptedNodeProperty();
}

// Generic Object
static void asPropertyGeneric_OpAssingCopy(asIScriptGeneric* gen)
{
    auto* object = reinterpret_cast<editor::QScriptedNodeProperty*>(gen->GetObject());
    auto* other = reinterpret_cast<editor::QScriptedNodeProperty*>(gen->GetArgObject(0));

    // Copy
    object->copyFrom(*other);
}

static void asPropertyGeneric_OpAssingValue(asIScriptGeneric* gen)
{
    auto* object = reinterpret_cast<editor::QScriptedNodeProperty*>(gen->GetObject());
    auto* value = *reinterpret_cast<asIScriptObject**>(gen->GetArgAddress(0));

    assert(object->subTypeId() == value->GetTypeId());
    // Store object
    //static_cast<QNodePinScriptTypeValue*>(object->pinValue())->setValue(value);
}

static void asPropertyGeneric_OpImplConv(asIScriptGeneric* gen)
{
    auto* object = reinterpret_cast<editor::QScriptedNodeProperty*>(gen->GetObject());
    //auto script_object = static_cast<QNodePinScriptTypeValue*>(object->pinValue())->valuePtr();
    //script_object->AddRef();

    //gen->SetReturnAddress(script_object);
}

void registerGenericPropertyType(asIScriptEngine* engine)
{
    // Generic property
    engine->RegisterObjectType("prop<class T>", QScriptedNodePropertyTypeSize, asOBJ_TEMPLATE | QScriptedNodePropertyTypeTraits);
    engine->RegisterObjectBehaviour("prop<T>", asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(asGenericPropertyTypeCheck), asCALL_CDECL);
    engine->RegisterObjectBehaviour("prop<T>", asBEHAVE_CONSTRUCT, "void f(int&in)", asFUNCTION(asPropertyGeneric_Construct), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour("prop<T>", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(asPropertyGeneric_Destruct), asCALL_CDECL_OBJLAST);

    // Objects
    engine->RegisterObjectMethod("prop<T>", "void opAssign(const prop<T>&in)", asFUNCTION(asPropertyGeneric_OpAssingCopy), asCALL_GENERIC);
    engine->RegisterObjectMethod("prop<T>", "void opAssign(const T&in if_handle_then_const)", asFUNCTION(asPropertyGeneric_OpAssingValue), asCALL_GENERIC);
    engine->RegisterObjectMethod("prop<T>", "const T& opImplConv() const", asFUNCTION(asPropertyGeneric_OpImplConv), asCALL_GENERIC);
}

//////////////////////////////////////////////////////////////////////////
// Specialized int property type (is used as for int values stored in properties)
template<class T> static int asTypeIdFromType() { return -1; }
template<> int asTypeIdFromType<bool>() { return asTYPEID_BOOL; }
template<> int asTypeIdFromType<int8_t>() { return asTYPEID_INT8; }
template<> int asTypeIdFromType<int16_t>() { return asTYPEID_INT16; }
template<> int asTypeIdFromType<int32_t>() { return asTYPEID_INT32; }
template<> int asTypeIdFromType<int64_t>() { return asTYPEID_INT64; }
template<> int asTypeIdFromType<uint8_t>() { return asTYPEID_UINT8; }
template<> int asTypeIdFromType<uint16_t>() { return asTYPEID_UINT16; }
template<> int asTypeIdFromType<uint32_t>() { return asTYPEID_UINT32; }
template<> int asTypeIdFromType<uint64_t>() { return asTYPEID_UINT64; }
template<> int asTypeIdFromType<float>() { return asTYPEID_FLOAT; }
template<> int asTypeIdFromType<double>() { return asTYPEID_DOUBLE; }

template<class T>
static void asPropertyNative_Construct(void* memory)
{
    new (memory) editor::QScriptedNodeProperty(nullptr, asTypeIdFromType<T>());
}

static void asPropertyNative_OpAssingCopy(editor::QScriptedNodeProperty* other, editor::QScriptedNodeProperty* object)
{
    if (object == other) return;
    //object->copyFrom(other);
    assert(false);
}

template<class T>
static void asPropertyNative_OpAssingValue(const T& value, editor::QScriptedNodeProperty* object)
{
    assert(object->subTypeId() == asTypeIdFromType<T>());
    object->nodePin()->value()->setValue(value);
}

template<class T>
T asPropertyNative_OpImplConv(editor::QScriptedNodeProperty* object)
{
    assert(object->subTypeId() == asTypeIdFromType<T>());
    return qvariant_cast<T>(object->nodePin()->value()->value());
}

template<class T>
void registerPropertyNative_Specialized(asIScriptEngine* engine, std::string name, std::string prop_name) { }

template<class T>
void registerNativePropertyType(asIScriptEngine* engine, std::string name)
{
    // Integer property
    std::string prop_type = "prop<" + name + ">";
    std::string prop_op_assing = "void opAssign(const " + prop_type + "&in)";
    std::string prop_op_assing_val = "void opAssign(const " + name + "&in)";
    std::string prop_op_impl_conv = name + " opImplConv() const";

    engine->RegisterObjectType(prop_type.c_str(), QScriptedNodePropertyTypeSize, QScriptedNodePropertyTypeTraits);
    engine->RegisterObjectBehaviour(prop_type.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(asPropertyNative_Construct<T>), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectBehaviour(prop_type.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(asPropertyGeneric_Destruct), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod(prop_type.c_str(), prop_op_assing.c_str(), asFUNCTION(asPropertyNative_OpAssingCopy), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod(prop_type.c_str(), prop_op_assing_val.c_str(), asFUNCTION(asPropertyNative_OpAssingValue<T>), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod(prop_type.c_str(), prop_op_impl_conv.c_str(), asFUNCTION(asPropertyNative_OpImplConv<T>), asCALL_CDECL_OBJLAST);

    // Specialized and additional methods to register
    registerPropertyNative_Specialized<T>(engine, name, prop_type);
}

//engine->RegisterObjectMethod("prop<int>", "int opAdd(const prop<int>&in) const", asFUNCTION(asPropertyGeneric_OpAdd), asCALL_GENERIC);
//engine->RegisterObjectMethod("prop<int>", "int opSub(const prop<int>&in) const", asFUNCTION(asPropertyGeneric_OpSub), asCALL_GENERIC);
//engine->RegisterObjectMethod("prop<int>", "int opMul(const prop<int>&in) const", asFUNCTION(asPropertyGeneric_OpMul), asCALL_GENERIC);
//engine->RegisterObjectMethod("prop<int>", "int opDiv(const prop<int>&in) const", asFUNCTION(asPropertyGeneric_OpDiv), asCALL_GENERIC);
//engine->RegisterObjectMethod("prop<int>", "int opMod(const prop<int>&in) const", asFUNCTION(asPropertyGeneric_OpMod), asCALL_GENERIC);

//////////////////////////////////////////////////////////////////////////
// SPecialized int property type (is used as for int values stored in properties)
void registerFloatPropertyType(asIScriptEngine* engine)
{
    //// Float property
    //engine->RegisterObjectType("prop<float>", sizeof(QScriptedNodeProperty), asOBJ_VALUE | asGetTypeTraits<QScriptedNodeProperty>());
    //engine->RegisterObjectBehaviour("prop<float>", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Editor::EQt::API::asPropertyFloat_Construct), asCALL_CDECL_OBJLAST);
    //engine->RegisterObjectBehaviour("prop<float>", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Editor::EQt::API::asPropertyGeneric_Destruct), asCALL_CDECL_OBJLAST);
    //engine->RegisterObjectMethod("prop<float>", "void opAssign(const prop<float>&in)", asFUNCTION(Editor::EQt::API::asPropertyFloat_OpAssingCopy), asCALL_CDECL_OBJLAST);
    //engine->RegisterObjectMethod("prop<float>", "void opAssign(const float&in)", asFUNCTION(Editor::EQt::API::asPropertyFloat_OpAssingValue), asCALL_CDECL_OBJLAST);
    //engine->RegisterObjectMethod("prop<float>", "float opImplConv() const", asFUNCTION(Editor::EQt::API::asPropertyFloat_OpImplConv), asCALL_CDECL_OBJLAST);
    //engine->RegisterObjectMethod("prop<float>", "float opAdd(const prop<float>&in) const", asFUNCTION(Editor::EQt::API::asPropertyGeneric_OpAdd), asCALL_GENERIC);
    //engine->RegisterObjectMethod("prop<float>", "float opSub(const prop<float>&in) const", asFUNCTION(Editor::EQt::API::asPropertyGeneric_OpSub), asCALL_GENERIC);
    //engine->RegisterObjectMethod("prop<float>", "float opMul(const prop<float>&in) const", asFUNCTION(Editor::EQt::API::asPropertyGeneric_OpMul), asCALL_GENERIC);
    //engine->RegisterObjectMethod("prop<float>", "float opDiv(const prop<float>&in) const", asFUNCTION(Editor::EQt::API::asPropertyGeneric_OpDiv), asCALL_GENERIC);

    //// Boolean property
    //engine->RegisterObjectType("prop<bool>", sizeof(QScriptedNodeProperty), asOBJ_VALUE | asGetTypeTraits<QScriptedNodeProperty>());
    //engine->RegisterObjectBehaviour("prop<bool>", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Editor::EQt::API::asPropertyBool_Construct), asCALL_CDECL_OBJLAST);
    //engine->RegisterObjectBehaviour("prop<bool>", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Editor::EQt::API::asPropertyGeneric_Destruct), asCALL_CDECL_OBJLAST);
    //engine->RegisterObjectMethod("prop<bool>", "void opAssign(const prop<bool>&in)", asFUNCTION(Editor::EQt::API::asPropertyBool_OpAssingCopy), asCALL_CDECL_OBJLAST);
    //engine->RegisterObjectMethod("prop<bool>", "void opAssign(const bool&in)", asFUNCTION(Editor::EQt::API::asPropertyBool_OpAssingValue), asCALL_CDECL_OBJLAST);
    //engine->RegisterObjectMethod("prop<bool>", "bool opImplConv() const", asFUNCTION(Editor::EQt::API::asPropertyBool_OpImplConv), asCALL_CDECL_OBJLAST);
}
//
//void Editor::EQt::asRegisterPropertyApi_v2(asIScriptEngine* engine)
//{
//    // Generic property
//    engine->RegisterObjectType("prop<class T>", sizeof(QNodePin), asOBJ_VALUE | asOBJ_TEMPLATE | asGetTypeTraits<QNodeProperty>());
//    engine->RegisterObjectBehaviour("prop<T>", asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(Editor::EQt::API::asGenericPropertyTypeCheck), asCALL_CDECL);
//    engine->RegisterObjectBehaviour("prop<T>", asBEHAVE_CONSTRUCT, "void f(int&in)", asFUNCTION(Editor::EQt::API::asPropertyGeneric_Construct), asCALL_CDECL_OBJLAST);
//    engine->RegisterObjectBehaviour("prop<T>", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Editor::EQt::API::asPropertyGeneric_Destruct), asCALL_CDECL_OBJLAST);
//}
//
//bool Editor::EQt::API::asGenericPropertyTypeCheck(asITypeInfo* typeinfo, bool& dont_gc)
//{
//    // This template will only support primitive (for now)
//    int typeId = typeinfo->GetSubTypeId();
//    if (typeId & asTYPEID_MASK_OBJECT)
//    {
//        if (typeId & asTYPEID_OBJHANDLE)
//        {
//            //auto subtype = typeinfo->GetSubType(0);
//            //auto engine = typeinfo->GetEngine();
//            return true;// subtype == engine->GetTypeInfoByName("INode");
//        }
//
//        return false;
//        //
//        //// The script is attempting to instantiate the
//        //// template with an object type, this is not allowed.
//        //return false;
//    }
//
//    dont_gc = true;
//    return true;
//}
//
////////////////////////////////////////////////////////////////////////////
//// Float
//void Editor::EQt::API::asPropertyFloat_Construct(void* memory)
//{
//    new (memory) QNodeProperty(asTYPEID_FLOAT); // asTYPEID_INT32, MemNew(EMem::Editor) QNodePinIntegerValue());
//}
//
//void Editor::EQt::API::asPropertyFloat_OpAssingCopy(QNodeProperty* other, QNodeProperty* object)
//{
//    if (object == other) return;
//    object->copyFrom(other);
//}
//
//void Editor::EQt::API::asPropertyFloat_OpAssingValue(const float& value, QNodeProperty* object)
//{
//    CHKRD(object->getTypeId() == asTYPEID_FLOAT);
//    object->pinValue()->setValue(value);
//}
//
//float Editor::EQt::API::asPropertyFloat_OpImplConv(QNodeProperty* object)
//{
//    CHKRD(object->getTypeId() == asTYPEID_FLOAT);
//    return object->pinValue()->getValue().toInt();
//}
//
////////////////////////////////////////////////////////////////////////////
//// Bool
//void Editor::EQt::API::asPropertyBool_Construct(void* memory)
//{
//    new (memory) QNodeProperty(asTYPEID_BOOL); // asTYPEID_INT32, MemNew(EMem::Editor) QNodePinIntegerValue());
//}
//
//void Editor::EQt::API::asPropertyBool_OpAssingCopy(QNodeProperty* other, QNodeProperty* object)
//{
//    if (object == other) return;
//    object->copyFrom(other);
//}
//
//void Editor::EQt::API::asPropertyBool_OpAssingValue(const bool& value, QNodeProperty* object)
//{
//    CHKRD(object->getTypeId() == asTYPEID_BOOL);
//    object->pinValue()->setValue(value);
//}
//
//bool Editor::EQt::API::asPropertyBool_OpImplConv(QNodeProperty* object)
//{
//    CHKRD(object->getTypeId() == asTYPEID_BOOL);
//    return object->pinValue()->getValue().toBool();
//}
//
////////////////////////////////////////////////////////////////////////////
//// Generic
//template<class T>
//inline T GetValueFromProperty(QNodeProperty* object, int arg)
//{
//    T result;
//    switch (object->getTypeId())
//    {
//    case asTYPEID_INT32:
//        result = object->pinValue()->getValue().toInt();
//        break;
//    case asTYPEID_FLOAT:
//        result = object->pinValue()->getValue().toFloat();
//        break;
//    case asTYPEID_BOOL:
//        result = object->pinValue()->getValue().toBool();
//        break;
//    default:
//        CHKIRD(FailAlways, "Unknown type!");
//    }
//    return result;
//}
//
//void Editor::EQt::API::asPropertyGeneric_OpAdd(asIScriptGeneric* gen)
//{
//    QNodeProperty* object = reinterpret_cast<QNodeProperty*>(gen->GetObject());
//    QNodeProperty* other = reinterpret_cast<QNodeProperty*>(gen->GetArgObject(0));
//
//    switch (object->getTypeId())
//    {
//    case asTYPEID_INT32:
//    {
//        gen->SetReturnDWord(object->pinValue()->getValue().toInt() + GetValueFromProperty<int>(other, 0));
//        break;
//    }
//    case asTYPEID_FLOAT:
//    {
//        gen->SetReturnFloat(object->pinValue()->getValue().toFloat() + GetValueFromProperty<float>(other, 0));
//        break;
//    }
//    case asTYPEID_BOOL:
//    {
//        gen->SetReturnByte(object->pinValue()->getValue().toBool() + GetValueFromProperty<bool>(other, 0));
//        break;
//    }
//    default:
//        CHKIRD(FailAlways, "Unknown type!");
//    }
//}
//
//void Editor::EQt::API::asPropertyGeneric_OpSub(asIScriptGeneric* gen)
//{
//    QNodeProperty* object = reinterpret_cast<QNodeProperty*>(gen->GetObject());
//    QNodeProperty* other = reinterpret_cast<QNodeProperty*>(gen->GetArgObject(0));
//
//    switch (object->getTypeId())
//    {
//    case asTYPEID_INT32:
//    {
//        gen->SetReturnDWord(object->pinValue()->getValue().toInt() - GetValueFromProperty<int>(other, 0));
//        break;
//    }
//    case asTYPEID_FLOAT:
//    {
//        gen->SetReturnFloat(object->pinValue()->getValue().toFloat() - GetValueFromProperty<float>(other, 0));
//        break;
//    }
//    case asTYPEID_BOOL:
//    {
//        gen->SetReturnByte(object->pinValue()->getValue().toBool() - GetValueFromProperty<bool>(other, 0));
//        break;
//    }
//    default:
//        CHKIRD(FailAlways, "Unknown type!");
//    }
//}
//
//void Editor::EQt::API::asPropertyGeneric_OpMul(asIScriptGeneric* gen)
//{
//    QNodeProperty* object = reinterpret_cast<QNodeProperty*>(gen->GetObject());
//    QNodeProperty* other = reinterpret_cast<QNodeProperty*>(gen->GetArgObject(0));
//
//    switch (object->getTypeId())
//    {
//    case asTYPEID_INT32:
//    {
//        gen->SetReturnDWord(object->pinValue()->getValue().toInt() * GetValueFromProperty<int>(other, 0));
//        break;
//    }
//    case asTYPEID_FLOAT:
//    {
//        gen->SetReturnFloat(object->pinValue()->getValue().toFloat() * GetValueFromProperty<float>(other, 0));
//        break;
//    }
//    case asTYPEID_BOOL:
//    {
//        gen->SetReturnByte(object->pinValue()->getValue().toBool() * GetValueFromProperty<bool>(other, 0));
//        break;
//    }
//    default:
//        CHKIRD(FailAlways, "Unknown type!");
//    }
//}
//
//void Editor::EQt::API::asPropertyGeneric_OpDiv(asIScriptGeneric* gen)
//{
//    QNodeProperty* object = reinterpret_cast<QNodeProperty*>(gen->GetObject());
//    QNodeProperty* other = reinterpret_cast<QNodeProperty*>(gen->GetArgObject(0));
//
//    switch (object->getTypeId())
//    {
//    case asTYPEID_INT32:
//    {
//        gen->SetReturnDWord(object->pinValue()->getValue().toInt() / GetValueFromProperty<int>(other, 0));
//        break;
//    }
//    case asTYPEID_FLOAT:
//    {
//        gen->SetReturnFloat(object->pinValue()->getValue().toFloat() / GetValueFromProperty<float>(other, 0));
//        break;
//    }
//    case asTYPEID_BOOL:
//    {
//        CHKIRD(FailAlways, "Cannot divide a bool value!");
//        break;
//    }
//    default:
//        CHKIRD(FailAlways, "Unknown type!");
//    }
//}
//
//void Editor::EQt::API::asPropertyGeneric_OpMod(asIScriptGeneric* gen)
//{
//    QNodeProperty* object = reinterpret_cast<QNodeProperty*>(gen->GetObject());
//    QNodeProperty* other = reinterpret_cast<QNodeProperty*>(gen->GetArgObject(0));
//
//    switch (object->getTypeId())
//    {
//    case asTYPEID_INT32:
//    {
//        gen->SetReturnDWord(object->pinValue()->getValue().toInt() % GetValueFromProperty<int>(other, 0));
//        break;
//    }
//    case asTYPEID_FLOAT:
//    {
//        CHKIRD(FailAlways, "Cannot modulo a float value!");
//        break;
//    }
//    case asTYPEID_BOOL:
//    {
//        CHKIRD(FailAlways, "Cannot modulo a bool value!");
//        break;
//    }
//    default:
//        CHKIRD(FailAlways, "Unknown type!");
//    }
//}
//
//void Editor::EQt::API::asPropertyGeneric_OpPow(asIScriptGeneric* gen)
//{
//    CHKIRD(FailAlways, "Not implemented!");
//}
