#pragma once
#include <QObject>
#include <QDir>

#include <memory>

namespace editor
{

//! The project class.
class QProject;

//! Handles all project operations in the application.
class QProjectManager final : public QObject
{
    Q_OBJECT;

public:
    QProjectManager() noexcept;
    ~QProjectManager() noexcept = default;

public slots:
    //! Opens the project in in the given location.
    //! \note If the project file exists, it will try to load the project before emitting the 'onProjectOpen' signal.
    void openProject(const QDir& location);

    //! Closes the given project.
    //! \note This function will call the 'onProjectClose' signal before saving it.
    void closeProject(QProject* project);

signals:
    //! Emitted when a project was successfully opened.
    void onProjectOpen(QProject* project);

    //! Emitted when a project was successfully closed.
    void onProjectClose(QProject* project);

private:
    using project_ptr = std::unique_ptr<QProject>;

    // List of all opened project objects.
    std::vector<project_ptr> _projects;
};

} // namespace editor
