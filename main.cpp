#include <QApplication>

#include "MainWindow.h"

int main (int argc, char* argv[])
{
    QApplication::setApplicationName("MarkdownViewer");
    QApplication::setApplicationDisplayName("Markdown Viewer");
    QApplication::setApplicationVersion (VERSION);
    QApplication::setOrganizationName (QStringLiteral ("Netherwood Industries"));

    QApplication a (argc, argv);
    MainWindow w;
    w.show ();
    return a.exec ();
}
