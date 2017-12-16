#pragma once
#include "graph/interfaces/QGraphSerializer.h"
#include "scripts/CScriptManager.h"

namespace editor
{
    class QScriptedGraphSerializer : public QGraphSerializer
    {
    public:
        QScriptedGraphSerializer();
        virtual ~QScriptedGraphSerializer() override;

        void initialize(Scripts::CScriptManager* script_manager);

        virtual bool serialize(QIODevice* io, QGraph* graph) override;
        virtual bool deserialize(QIODevice* io, QGraph* graph) override;

    private:
        Scripts::CScriptManager* _script_manager;
    };
}