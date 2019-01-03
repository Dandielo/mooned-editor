#include "editor.h"

#include "windows/QEditorMainWindow.h"

#include <QAction>
#include <QDialog>
#include <QMessageBox>

#include <QApplication>
#include <QCoreApplication>
#include <QStyleFactory>

int main(int argc, char** argv)
{
    int result;
    {
        QApplication app(argc, argv);
        app.setApplicationName("MEditor");
        app.setStyle(QStyleFactory::create("Fusion"));

        editor::QEditorMainWindow mw;
        mw.show();

        result = app.exec();
    }

    return result;
}
