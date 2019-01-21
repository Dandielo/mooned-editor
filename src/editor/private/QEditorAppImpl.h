#include <meditor/QEditorApp.h>
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

    //! Runs the main event loop of the application.
    //! \returns The result of the application runtime.
    int execute() noexcept;

private:
    //! The qt application.
    QApplication _app_native;

    //! The application settings.
    std::unique_ptr<QPersistentSettings> _app_settings;
};

//! Holds the instance of the editor application.
extern QEditorAppImpl* editor_application_instance;

} // namespace editor::detail
