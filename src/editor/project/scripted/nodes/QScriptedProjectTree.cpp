#include <project/scripted/nodes/QScriptedProjectTree.h>
#include <project/scripted/QScriptedProject.h>

namespace editor
{

ScriptedProjectTreeRoot::ScriptedProjectTreeRoot(QScriptedProject* project) noexcept
    : ProjectTreeRoot{ project }
{ }

} // namespace editor
