#pragma once
#include "project/interfaces/QProjectExporter.h"
#include "scripts/CNativeScriptObject.h"

namespace editor
{
    class QProject;
    class QProjectElement;

    class QScriptedProjectExporter : public QProjectExporter, public Scripts::CNativeScriptObject<QScriptedProjectExporter>
    {
        M_SCRIPT_TYPE(QScriptedProjectExporter, "CExporter");

    public:
        QScriptedProjectExporter(asIScriptObject* object);
        virtual ~QScriptedProjectExporter() override;

        virtual void serialize(QProject* project) override;
    };
}
