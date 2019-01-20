#pragma once
#include "graph/scripted/QScriptedGraphSerializer.h"
#include "project/interfaces/QProjectElement.h"
#include "scripts/CScriptManager.h"

#include <settings/QSettings.h>

namespace editor
{

//! The QScriptedGraph class.
class QScriptedGraph;

//! The QScriptedProject class.
class QScriptedProject;

//! A project element class which is used to hold scripted graph objects.
class QScriptedElementGraph : public QProjectElement
{
    Q_OBJECT;

public:
    //! The old constructor for scripted graph elements.
    //QScriptedElementGraph(QScriptedProject* project, QString classname, QString name);

    //! The new constructor for scripted graph elements.
    QScriptedElementGraph(QProjectElement* parent, QString classname, QFileInfo graph_file) noexcept;

    //! The even newer constructor for scripted graph elements.
    QScriptedElementGraph(QProjectElement* parent, QString graph_class, QString graph_name) noexcept;

    void initialize(Scripts::CScriptManager* script_manager);
    void shutdown();

    //! \returns The tree node type.
    auto type() const noexcept -> Type { return Type::Graph; }

    //! \returns The script class name which this element holds.
    auto class_name() const noexcept -> const QString& { return _class_name; }

    //! Loads graph data from file.
    void load() noexcept override;

    //! Saves graph data to file.
    void save() const noexcept override;

    //! \returns The project settings object.
    auto settings() noexcept -> QSettings& { return _settings; }
    auto settings() const noexcept -> const QSettings& { return _settings; }

    //! \returns A value for the given item data role.
    auto value(Qt::ItemDataRole role) const noexcept -> QVariant override;

public:
    editor::QScriptedGraph* graph() const { return _graph; }

private:
    //! Holds the script class name of this element.
    const QString _class_name;

    //! Holds specific setting for this element.
    QSettings _settings;

    Scripts::CScriptManager* _script_manager{ nullptr };
    editor::QScriptedGraph* _graph{ nullptr };
};

} // namespace editor
