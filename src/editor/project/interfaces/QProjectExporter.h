#pragma once

namespace editor
{
    class QProject;
    class QProjectElement;

    class QProjectExporter
    {
    public:
        QProjectExporter() = default;
        virtual ~QProjectExporter() = default;

        virtual void serialize(QProject* project) = 0;
    };
}
