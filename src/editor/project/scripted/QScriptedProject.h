#pragma once
#include "project/interfaces/QProject.h"
#include "scripts/CScriptManager.h"

class QEditorMainWindow;

namespace editor
{
    class QProjectModel;
    class QProjectTree;

    class QScriptedGraph;
    class QScriptedProjectTree;

    class QScriptedProject : public QProject
    {
        Q_OBJECT;

    public:
        QScriptedProject();
        QScriptedProject(QString name, QDir location);
        virtual ~QScriptedProject() override;

        void initialize(QEditorMainWindow* mw) override;
        void initialize(Scripts::CScriptManager* script_manager);
        void shutdown();

        virtual void newGraph(QString classname, QString name) override;

        virtual void openElement(QString name) override;
        virtual void saveElement(QString name) override;
        virtual void deleteElement(QString name) override;

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