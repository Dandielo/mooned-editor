#include "QEditorAppImpl.h"
#include "windows/QEditorMainWindow.h"

using editor::QEditorMainWindow;
using editor::detail::QEditorAppImpl;

int main(int argc, char** argv)
{
    QEditorAppImpl app{ argc, argv };

    int result = 0;
    if (app.inGuiMode())
    {
        QEditorMainWindow main_window;
        main_window.show();

        // Run the main loop.
        result = app.execute();
    }
    else
    {
        // Do nothing for now?
    }

    return result;
}
