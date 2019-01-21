#pragma once
#include <interfaces/QWorkspace.h>
#include <meditor/settings/QPersistentSettings.h>
#include <project/interfaces/QProjectElement.h>

#include <QVersionNumber>
#include <QObject>
#include <QDir>


namespace editor
{

//! The editor main window class.
class QEditorMainWindow;

//! The base class for all project types in this editor application.
//! \note Creating a QProject object does always create a valid object.
class QProject : public QProjectElement
{
    Q_OBJECT;

public:
    //! A project is nothing more than a specific json file.
    QProject(const QFileInfo& project_file) noexcept;

    //! virtual dtor
    virtual ~QProject() noexcept = default;

    //! \returns The tree node type.
    auto type() const noexcept -> Type override final { return Type::Project; }

    //! Initializes the project element.
    virtual void initialize(editor::QEditorMainWindow* editor) noexcept = 0;

    //! \returns The projects class name.
    virtual auto className() const noexcept -> QString = 0;

    //! \returns The projects current version.
    virtual auto version() const noexcept -> QVersionNumber = 0;

    //! Tries to load project information for associated the project file.
    void load() noexcept override;

    //! Tries to save project information into associated the project file.
    void save() const noexcept override;

    //! \returns The project location.
    auto location() const noexcept -> const QDir& { return _location; }

    //! \returns The project settings object.
    auto settings() noexcept -> QSettings& { return _settings; }
    auto settings() const noexcept -> const QSettings& { return _settings; }

private:
    //////////////////////////////////////////////////////////////////////////
    // Implements the QProjectElement::project methods.
    auto project() noexcept -> QProject* override { return this; }
    auto project() const noexcept -> const QProject* override { return this; }


public:
    virtual void openElement(QString name) = 0;
    virtual void saveElement(QString name) = 0;
    virtual void deleteElement(QString name) = 0;
    virtual void exportElement(QString name) = 0;

public slots:
    virtual void newGraph(QString classname, QString name) = 0;

protected:
    virtual void onSave(QJsonObject& root) const = 0;
    virtual void onLoad(const QJsonObject& root, const QVersionNumber&) = 0;

private:
    //! The project location.
    QDir _location;

    //! All project settings values.
    QPersistentSettings _settings;
};

} // namespace editor::project
