#pragma once

namespace editor
{

//! The settings class.
class QSettings;

//! A interface for the editor application instance.
class QEditorApp
{
public:
    virtual ~QEditorApp() noexcept = default;

    //! \returns  The current running instance of the editor application.
    //! \pre The application is initialized and running in the main loop.
    static auto instance() noexcept -> QEditorApp&;

    //! \returns The application settings object.
    virtual auto settings() noexcept -> QSettings& = 0;
    virtual auto settings() const noexcept -> const QSettings& = 0;
};

} // namespace editor
