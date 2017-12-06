#include "QScriptedNode.h"

using editor::QScriptedNode;

void asNodeSetPosProxy(QScriptedNode* node, int x, int y)
{
    node->setPos({ static_cast<float>(x), static_cast<float>(y) });
}

void asNodeSetColorProxy(QScriptedNode* node, double r, double g, double b)
{
    node->setColor(QColor::fromRgbF(r, g, b));
}

void asNodeSetNameProxy(QScriptedNode* node, std::string name)
{
    node->setName(QString::fromStdString(name));
}

std::string asNodeGetNameProxy(QScriptedNode* node)
{
    return node->name().toLocal8Bit().data();
}

void asNodeSetDescriptionProxy(QScriptedNode* node, std::string name)
{
    node->setDesc(QString::fromStdString(name));
}

std::string asNodeGetDescriptionProxy(QScriptedNode* node)
{
    return node->desc().toLocal8Bit().data();
}

void QScriptedNode::registerTypeInterface(asIScriptEngine* engine)
{
    //static_assert(sizeof(double) == sizeof(qreal), "'qreal' type is incompatible with the 'double' type");
    engine->RegisterObjectMethod(ClassName, "void move(int, int)", asFUNCTION(asNodeSetPosProxy), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod(ClassName, "void color(double, double, double)", asFUNCTION(asNodeSetColorProxy), asCALL_CDECL_OBJFIRST);

    // Protected properties
    engine->RegisterObjectMethod(ClassName, "string get_name()", asFUNCTION(asNodeGetNameProxy), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod(ClassName, "void set_name(string)", asFUNCTION(asNodeSetNameProxy), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod(ClassName, "string get_description()", asFUNCTION(asNodeGetDescriptionProxy), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod(ClassName, "void set_description(string)", asFUNCTION(asNodeSetDescriptionProxy), asCALL_CDECL_OBJFIRST);

    // Public interface
    engine->RegisterInterface("INode");
    engine->RegisterInterfaceMethod("INode", "void move(int, int)");
    engine->RegisterInterfaceMethod("INode", "void color(double, double, double)");

    engine->RegisterInterfaceMethod("INode", "string get_name()");
    engine->RegisterInterfaceMethod("INode", "void set_name(string)");
    engine->RegisterInterfaceMethod("INode", "string get_description()");
    engine->RegisterInterfaceMethod("INode", "void set_description(string)");

    // Handles
    engine->RegisterInterfaceMethod("INode", "void OnCreate()");
    engine->RegisterInterfaceMethod("INode", "void OnDestroy()");
}
