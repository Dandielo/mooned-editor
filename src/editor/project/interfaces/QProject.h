#pragma once
#include "interfaces/QWorkspace.h"

#include <QString>
#include <QObject>
#include <QDir>

namespace editor
{
    class QProject : public QObject
    {
        Q_OBJECT;

    public:
        QProject();
        QProject(QString name, QDir location);
        virtual ~QProject();

        QString name() const;

        bool isValid() const;

        bool open(QFileInfo location);
        bool save();

    protected:
        QString _name;
        QString _filename;
        QDir _location;
    };
}