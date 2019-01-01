#pragma once
#include "project/interfaces/QProjectExporter.h"
#include "scripts/CNativeScriptObject.h"

#include <sstream>

namespace editor
{

//! The script node type.
class QScriptedNode;

//! A AngelScript defined project exporter.
class QScriptedProjectExporter : public QProjectExporter, public Scripts::CNativeScriptObject<QScriptedProjectExporter>
{
    M_SCRIPT_TYPE(QScriptedProjectExporter, "CExporter");

public:
    //! The constructor takes the script related object as input.
    QScriptedProjectExporter(editor::script::ScriptObject&& object);

    //! Saves exported files on destruction?
    ~QScriptedProjectExporter() noexcept override;

    //! Serializes the given project object.
    //! \note Only object instances of the QScriptedProject are serialized.
    void export_project(QProject* project) const noexcept override;

    //! Serializes the given QScriptedGraph object.
    void export_project_element(QProjectElement* graph) const noexcept;

protected:
    //! Writes string of text into the buffer.
    void write(const std::string& str) noexcept;

    //! Writes line into the buffer.
    void writeln(const std::string& str) noexcept;

    //! Saves the current buffer state under the given file name.
    //! \note This function clears the buffer.
    void flush(const std::string& file_name) noexcept;

protected:
    std::stringstream _stream{ };
};

}
