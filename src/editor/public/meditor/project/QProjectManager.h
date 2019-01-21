#pragma once
#include <QObject>

namespace editor
{

//! The project class.
class QProject;

//! Handles all project operations in the application.
class QProjectManager final : public QObject
{
public:
    QProjectManager() noexcept;
    ~QProjectManager() noexcept = default;
};

} // namespace editor
