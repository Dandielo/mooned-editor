#pragma once 
#include "scripts/CScriptManager.h"

namespace editor
{
    class QScriptedNode;
    class QScriptedNodePin;

    class QScriptedNodeProperty final
    {
    public:
        static void registerType(asIScriptEngine* engine);

        QScriptedNodeProperty(asITypeInfo* type, int subtype_id);
        ~QScriptedNodeProperty();

        void initialize(Scripts::CScriptManager* manager, QScriptedNode* node, int index);

        int typeId() const { return _type->GetTypeId(); }
        int subTypeId() const { return _subtype_id; }

        QScriptedNodePin* nodePin() const { return _node_pin; }

        void copyFrom(const QScriptedNodeProperty& other);

    private:
        asITypeInfo* const _type;
        const int _subtype_id;

        QScriptedNodePin* _node_pin;
    };
}
