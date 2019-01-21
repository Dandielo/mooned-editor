#pragma once
#include <meditor/settings/QPersistentSettings.h>

#include <project/interfaces/QProject.h>
#include <project/models/QProjectModel.h>

#include <windows/QWorkspaceWindow.h>
#include <scripts/CScriptManager.h>

#include <QMainWindow>
#include <QFileInfo>
#include <QPointer>

#include <memory>

#include "ui_mainwindow.h"

namespace Ui
{

//! The Ui generated class.
class QTWindow;

}

namespace editor
{
namespace project
{

//! Data passed to the factory function.
struct FactoryData
{
    std::string class_name;
    std::string name;
    QFileInfo file;
};

//! Alias for the project factory function signature.
using FactorySignature = auto(Scripts::CScriptManager*, const FactoryData&) -> QProject*;

//! Alias for the project factory function pointer.
using FactoryFunction = std::function<FactorySignature>;

} // namespace project

//! The editors main window class
class QEditorMainWindow : public QMainWindow
{
    Q_OBJECT;

public:
    //! ctor
    QEditorMainWindow() noexcept;

    //! \returns The map of all registered project factories.
    auto projectFactories() const noexcept -> const auto& { return _project_factories; }

    //! Adds a project factory for the given class name.
    void addProjectFactory(const std::string& class_name, editor::project::FactoryFunction factory) noexcept;

    //! \returns The project factory for the requested project class name.
    auto getProjectFactory(const std::string& class_name) const noexcept -> const editor::project::FactoryFunction&;

public slots:
    // Project action slots
    void onOpenProject();
    void onSaveProject();

    void onSaveProject(QString name);
    void onCloseProject(QString name);

    //! A slot for adding a project to the application
    void openProject(const editor::project::FactoryData& data) noexcept;

private:
    //! Alias of a unique pointer type for QWorkspaceWindow objects.
    using QWorkspaceWindowPtr = std::unique_ptr<QWorkspaceWindow, void(*)(QWorkspaceWindow*) noexcept>;

    //! Alias of a unique pointer type for QProject objects.
    using QProjectPtr = std::unique_ptr<QProject, void(*)(QProject*) noexcept>;

private:
    //! Holds the Ui class describing this main window.
    const std::unique_ptr<Ui::QTWindow> _ui;

    //! Holds the script manager used by this editor.
    const std::unique_ptr<Scripts::CScriptManager> _script_manager;

    //! Holds the model used to represent project trees in the editor.
    std::unique_ptr<QProjectModel> _project_model{ nullptr };

    //! Holds the master window for workspaces.
    QWorkspaceWindowPtr _workspace_window{ nullptr, nullptr /* deleter */ };

    //! Holds a map of registered project factories.
    std::map<std::string, project::FactoryFunction> _project_factories{ };

    //! Holds a vector of all currently opened projects.
    std::vector<QProjectPtr> _projects{ };

    //! Holds the current active project.
    QProject* _active_project{ nullptr };
};

} // namespace editor
