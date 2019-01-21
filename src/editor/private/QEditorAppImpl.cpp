#include "QEditorAppImpl.h"
#include <meditor/settings/QPersistentSettings.h>

#include <QStandardPaths>
#include <QStyleFactory>
#include <QDir>

#include <cassert>

namespace editor
{
namespace detail
{

QEditorAppImpl::QEditorAppImpl(int argc, char** argv) noexcept
    : _app_native{ argc, argv }
{
    editor_application_instance = this;

    // Set the default application values.
    _app_native.setApplicationName("MEditor");
    _app_native.setStyle(QStyleFactory::create("Fusion"));

    // Create the settings object.
    _app_settings = std::make_unique<QPersistentSettings>(QDir{ QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) });

    // Set the default values.
    _app_settings->set_default("projects.location", QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/meditor/projects"));

    // Initialize remaining things.
    QDir{ _app_settings->get("projects.location").toString() }.mkpath(".");
}

QEditorAppImpl::~QEditorAppImpl() noexcept
{
    editor_application_instance = nullptr;
}

auto QEditorAppImpl::settings() noexcept -> QSettings&
{
    return *_app_settings;
}

auto QEditorAppImpl::settings() const noexcept -> const QSettings&
{
    return *_app_settings;
}

int QEditorAppImpl::execute() noexcept
{
    return _app_native.exec();
}

//! Definition of the private global application instance variable.
QEditorAppImpl* editor_application_instance = nullptr;

} // namespace detail

auto QEditorApp::instance() noexcept -> QEditorApp&
{
    assert(detail::editor_application_instance != nullptr);
    return *detail::editor_application_instance;
}

} // namespace editor
