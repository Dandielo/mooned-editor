#pragma once
#include <interfaces/QWorkspace.h>
#include <settings/QPersistentSettings.h>
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
    QProject(QFileInfo project_file) noexcept;

    //! virtual dtor
    virtual ~QProject() noexcept = default;

    //! Initializes the project element.
    virtual void initialize(editor::QEditorMainWindow* editor) noexcept = 0;

    //! \returns The projects class name.
    virtual auto class_name() const noexcept -> QString = 0;

    //! \returns The projects current version.
    virtual auto version() const noexcept -> QVersionNumber = 0;

    //! \returns The project settings object.
    auto settings() noexcept -> QSettings& { return _settings; }
    auto settings() const noexcept -> const QSettings& { return _settings; }

    //! Tries to load project information for associated the project file.
    void load() noexcept override;

    //! Tries to save project information into associated the project file.
    void save() const noexcept override;


public:
    //! Alias for project element pointers.
    using QProjectElementPtr = QPointer<QProjectElement>;

    //! Adds the given element to the project.
    //! \param[in] element The element to be added to the project.
    //! \returns true If the element was added to the project.
    bool add_element(QProjectElementPtr element) noexcept;

    //! Tries to remove the given element from the project.
    //! \param[in] identifier A string value representing a unique id the the project.
    //! \note If the element is not part of the project, nothing will happen.
    void remove_element(const QString& identifier) noexcept;

    //! Searches for an element of the given identifier and returns a pointer to it.
    //! \param[in] identifier A string value representing a unique id the the project.
    //! \returns A pointer to the requested element or nullptr if nothing was found.
    auto get_element(const QString& identifier) noexcept -> QProjectElementPtr;
    auto get_element(const QString& identifier) const noexcept -> const QProjectElementPtr;

    //! Checks if the project contains an element with the given identifier.
    //! \param[in] identifier A string value representing a unique id the the project.
    bool contains_element(const QString& identifier) const noexcept;

    //! \returns A list of all elements in the project.
    auto elements() noexcept -> QMap<QString, QProjectElementPtr>& { return _elements; }
    auto elements() const noexcept -> const QMap<QString, QProjectElementPtr>& { return _elements; }

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
    //! All project settings values.
    QPersistentSettings _settings;

    //! Holds all elements part of this project.
    QMap<QString, QProjectElementPtr> _elements{ };
};

} // namespace editor::project
