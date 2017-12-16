#pragma once
#include "graph/scripted/QScriptedGraphSerializer.h"
#include "project/interfaces/QProjectElement.h"
#include "scripts/CScriptManager.h"

namespace editor
{
    class QScriptedGraph;
    class QScriptedProject;
    class QScriptedElementGraph : public QProjectElement
    {
        Q_OBJECT;

    public:
        QScriptedElementGraph(QScriptedProject* project, QString classname, QString name);
        virtual ~QScriptedElementGraph() override;

        void initialize(Scripts::CScriptManager* script_manager);
        void shutdown();

        bool initialized() const { nullptr != _script_manager && nullptr != _graph; }

        QString graphName() const { return _name; }
        QString className() const { return _classname; }

        virtual QString name() const override;
        virtual QString displayText() const override;

        virtual void save() override;
        virtual void load() override;

    public:
        editor::QScriptedGraph* graph() const { return _graph; }

    protected:
        QString fileLocation() const;

    private:
        editor::QScriptedGraphSerializer _graph_serializer;
        Scripts::CScriptManager* _script_manager;
        editor::QScriptedGraph* _graph;

        QString _classname;
        QString _name;
    };
}