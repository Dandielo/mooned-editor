#include <meditor/QEditorApp.h>
#include <meditor/project/QProjectManager.h>
#include <meditor/settings/QPersistentSettings.h>

#include <QApplication>

#include <memory>

namespace editor::detail
{

//! The editor application implementation.
class QEditorAppImpl final : public QEditorApp
{
public:
    QEditorAppImpl(int argc, char** argv) noexcept;
    ~QEditorAppImpl() noexcept override;

    //! \returns True if the application is running in gui mode.
    //! \note Always true.
    bool inGuiMode() const noexcept { return true; }

    //! \returns The application settings object.
    auto settings() noexcept -> QSettings& override;
    auto settings() const noexcept -> const QSettings& override;

    //! \returns The application project manager object.
    auto projectManager() noexcept -> QProjectManager& override { return _project_manager; }
    auto projectManager() const noexcept -> const QProjectManager& override { return _project_manager; }

    //! Runs the main event loop of the application.
    //! \returns The result of the application runtime.
    int execute() noexcept;

private:
    //! The qt application.
    QApplication _app_native;

    //! The application project manager.
    QProjectManager _project_manager{ };

    //! The application settings.
    std::unique_ptr<QPersistentSettings> _app_settings{ nullptr };
};

//! Holds the instance of the editor application.
extern QEditorAppImpl* editor_application_instance;

} // namespace editor::detail
