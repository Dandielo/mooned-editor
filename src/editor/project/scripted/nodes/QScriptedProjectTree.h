#pragma once
#include <project/models/QProjectTreeRoot.h>

namespace editor
{

class QScriptedProject;

//! A project tree root for the QScriptedProject object.
class ScriptedProjectTreeRoot : public editor::ProjectTreeRoot
{
public:
    ScriptedProjectTreeRoot(QScriptedProject* project) noexcept;
};

} // namespace editor
