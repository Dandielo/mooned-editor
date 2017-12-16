#pragma once
#include "interfaces/QWorkspace.h"

#include <QMap>
#include <QString>
#include <QObject>
#include <QDir>

class QEditorMainWindow;

namespace editor
{
    class QProjectElement;

    class QProject : public QObject
    {
        Q_OBJECT;

    public:
        QProject();
        QProject(QString name, QDir location);
        virtual ~QProject();

        virtual void initialize(QEditorMainWindow* mw) = 0;

        QString name() const;

        bool isValid() const;

        bool open(QFileInfo location);
        bool save();

        bool hasElement(QString name) const;
        bool hasElement(QProjectElement* element);
        void addElement(QProjectElement* element);
        void removeElement(QProjectElement* element);

    public slots:
        virtual void newGraph(QString classname, QString name) = 0;

    protected:
        virtual void onSave(QJsonObject& root) = 0;
        virtual void onLoad(const QJsonObject& root) = 0;

    protected:
        QString _name;
        QString _filename;
        QDir _location;

        QMap<QString, QProjectElement*> _elements;
    };
}