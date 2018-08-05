#pragma once
#include "interfaces/QWorkspace.h"

#include <QMap>
#include <QString>
#include <QObject>
#include <QDir>
#include <QJsonDocument>

class QEditorMainWindow;

namespace editor
{

class QProject;

using TProjectFactory = editor::QProject*(*)(const QString& type, const QString& name, const QString& project_file, void* userdata);

struct SProjectTypeEntry
{
    TProjectFactory factory;
    void* userdata;
};

class QProjectElement;

class QProject : public QObject
{
    Q_OBJECT;

public:
    QProject();
    virtual ~QProject();

    QString name() const;
    QString type() const;
    QString filename() const;
    QDir location() const;

    bool isValid() const;

    bool open(QFileInfo location);
    bool save();

    bool hasElement(QString name) const;
    bool hasElement(QProjectElement* element);
    void addElement(QProjectElement* element);
    void removeElement(QProjectElement* element);

    virtual void openElement(QString name) = 0;
    virtual void saveElement(QString name) = 0;
    virtual void deleteElement(QString name) = 0;
    virtual void exportElement(QString name) = 0;

    virtual void initialize(QEditorMainWindow* mw) = 0;

public slots:
    virtual void newGraph(QString classname, QString name) = 0;

protected:
    virtual void onSave(QJsonObject& root) = 0;
    virtual void onLoad(const QJsonObject& root) = 0;

protected:
    QString _name;
    QString _type;
    QString _filename;
    QDir _location;

    QMap<QString, QProjectElement*> _elements;
};

QJsonDocument loadProjectFile(QFileInfo path);

}