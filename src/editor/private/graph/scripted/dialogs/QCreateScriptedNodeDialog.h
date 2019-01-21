#pragma once
#include <scripts/angelscript_new/type.h>

#include <QDialog>

class asITypeInfo;

namespace Ui
{
    class DialogWithList;
}

namespace editor
{
    class QScriptedGraph;
    class QScriptedGraphNodeTreeModel;

    class QCreateScriptedNodeDialog : public QDialog
    {
        Q_OBJECT;

    public:
        QCreateScriptedNodeDialog(QScriptedGraph* graph, QVector<editor::script::Type> node_types, QWidget* parent = nullptr);
        virtual ~QCreateScriptedNodeDialog() override;

    signals:
        void finished(QString classname);

    protected slots:
        void createNodeAction();

    protected:
        virtual void closeEvent(QCloseEvent* ev) override;

    private:
        Ui::DialogWithList* _ui;
        QScriptedGraph* _graph;
        QScriptedGraphNodeTreeModel* _model;
    };
}
