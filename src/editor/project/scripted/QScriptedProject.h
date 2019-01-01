#pragma once
#include <project/interfaces/QProject.h>
#include <scripts/CScriptManager.h>
#include <scripts/CNativeScriptObject.h>

#include <memory>

class QEditorMainWindow;

namespace editor
{

class QProjectModel;
class QScriptedGraph;

class ProjectTreeRoot;

//! A project implementation based on the AngelScript language.
//! \note This class is partially defined in a script file and cannot be created outside the script engine.
class QScriptedProject : public QProject, public Scripts::CNativeScriptObject<QScriptedProject>
{
    M_SCRIPT_TYPE(QScriptedProject, "CProject");
    Q_OBJECT;

public:
    //! Alias for the script factory structure for this type.
    using FactoryData = editor::script::FactoryUserdata<QScriptedProject>;

public:
    QScriptedProject(editor::script::ScriptObject&& object, QFileInfo project_file, QString script_class);

    //! \returns The projects script class name.
    auto class_name() const noexcept -> QString override;

    void setScriptManager(Scripts::CScriptManager* script_manager);

    void initialize(QEditorMainWindow* mw) noexcept override;

    void shutdown();

    virtual void newGraph(QString classname, QString name) override;

    virtual void openElement(QString name) override;
    virtual void saveElement(QString name) override;
    virtual void deleteElement(QString name) override;
    virtual void exportElement(QString name) override;

signals:
    void projectTreeChanged(ProjectTreeRoot* tree);
    void graphOpened(editor::QScriptedGraph* graph);

protected:
    virtual void onSave(QJsonObject& root) const override;
    virtual void onLoad(const QJsonObject& root) override;

    // Loading helpers
    void addGraph(QString classname, QString name);

private:
    // Calls the 'AvailableExporters' method to gather a list of exporter types.
    // \note Validates the gathered types with the script manager.
    void gatherExporters() noexcept;

private:
    const QString _script_class;

    // A map of exporter classes possible to be used with this project.
    QVector<QString> _exporters;

    // The global script manager
    Scripts::CScriptManager* _script_manager{ nullptr };

    // The project tree (with graphs etc.)
    std::unique_ptr<editor::ProjectTreeRoot> _project_tree{ nullptr };

    // The project model
    QProjectModel* _model{ nullptr };
};

//////////////////////////////////////////////////////////////////////////

namespace script
{

//! Specialization for the factory userdata structure.
template<>
struct FactoryUserdata<QScriptedProject>
{
    QFileInfo file;
    QString type;
};

} // namespace script
} // namespace editor
