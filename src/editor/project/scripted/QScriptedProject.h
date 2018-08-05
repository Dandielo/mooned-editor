#pragma once
#include "project/interfaces/QProject.h"
#include "scripts/CScriptManager.h"
#include "scripts/CNativeScriptObject.h"

class QEditorMainWindow;

namespace editor
{
    class QProjectModel;
    class QProjectTree;

    class QScriptedGraph;
    class QScriptedProjectTree;

    class QScriptedProject : public QProject, public Scripts::CNativeScriptObject<QScriptedProject>
    {
        M_SCRIPT_TYPE(QScriptedProject, "CProject");
        Q_OBJECT;

    public:
        QScriptedProject(asIScriptObject* object);
        virtual ~QScriptedProject() override;

        void setScriptManager(Scripts::CScriptManager* script_manager);

        void setup(QString type, QString project_name);
        void initialize(QEditorMainWindow* mw) override;

        void shutdown();

        virtual void newGraph(QString classname, QString name) override;

        virtual void openElement(QString name) override;
        virtual void saveElement(QString name) override;
        virtual void deleteElement(QString name) override;
        virtual void exportElement(QString name) override;

    signals:
        void projectTreeChanged(QProjectTree* tree);
        void graphOpened(editor::QScriptedGraph* graph);

    protected:
        virtual void onSave(QJsonObject& root) override;
        virtual void onLoad(const QJsonObject& root) override;

        // Loading helpers
        void addGraph(QString classname, QString name);

    private:
        Scripts::CScriptManager* _script_manager;
        QScriptedProjectTree* _project_tree;
        QProjectModel* _model;
    };
}