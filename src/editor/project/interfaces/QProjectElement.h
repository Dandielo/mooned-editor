#pragma once
#include <QObject>
#include <QFileInfo>

namespace editor
{
    class QProject;
    class QProjectElement : public QObject
    {
        Q_OBJECT;

    public:
        QProjectElement(QProject* project);
        virtual ~QProjectElement() override;

        virtual QString name() const = 0;
        virtual QString displayText() const;

        virtual void save();
        virtual void load();

    protected:
        QProject* _project; //#todo is it needed?
    };
}
