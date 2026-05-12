#include <QApplication>
#include <QCommandLineParser>
#include <QStyleFactory>

#include "MainWindow.h"

int main (int argc, char* argv[])
{
    QApplication::setApplicationName (QStringLiteral ("MarkdownViewer"));
    QApplication::setApplicationDisplayName (QStringLiteral ("Markdown Viewer"));
    QApplication::setApplicationVersion (VERSION);
    QApplication::setOrganizationName (QStringLiteral ("Netherwood Industries"));

    QApplication a (argc, argv);
    a.setStyle (QStyleFactory::create ("Fusion"));

    QCommandLineParser parser;
    parser.setApplicationDescription (QStringLiteral ("Viewer for markdown files (*.md)"));
    parser.addHelpOption ();
    parser.addVersionOption ();
    parser.addPositionalArgument (QStringLiteral ("md"), QStringLiteral ("Markdown file to view (*.md"));
    parser.process (a);

    MainWindow w;
    w.show ();

    if (parser.positionalArguments ().count () == 1)
    {
        w.load (parser.positionalArguments ().constFirst (), false);
    }
    return a.exec ();
}
