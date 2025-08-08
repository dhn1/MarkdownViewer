#include "About.h"

#include <QProcess>
#include <QStandardPaths>
#include <QtGlobal>

#include "LibUiVersion.h"
#include "ui_About.h"

QString About::meshCheckerVersion ()
{
    QProcess proc (this);

#if defined QT_DEBUG
    // We need MeshChecker to pick up the installed library, not whatever we are running during debug of viewer
    auto env = QProcessEnvironment::systemEnvironment ();
    env.remove (QStringLiteral ("LD_LIBRARY_PATH"));
    proc.setProcessEnvironment (env);
#endif

    auto exe = QStandardPaths::findExecutable (QStringLiteral ("MeshChecker"));
    if (exe.isEmpty ())
    {
        return {};
    }

    QStringList args;
    args << QStringLiteral ("--version");
    proc.start (exe, args);
    proc.waitForFinished ();
    if (proc.exitCode() == 0)
    {
        auto ret = "\n" +  proc.readAllStandardOutput ();
        ret.replace (" ", ": " );
        return ret;
    }
    return {};
}

About::About (QWidget* parent) : QDialog (parent), ui (new Ui::About)
{
    ui->setupUi (this);
    ui->versionLabel->setText ("Markdown Viewer: " + qApp->applicationVersion () + "\nlibui: " + libUiVersion () + QStringLiteral ("\nQt: %1.%2.%3").arg (QT_VERSION_MAJOR).arg (QT_VERSION_MINOR).arg (QT_VERSION_PATCH) +
        meshCheckerVersion ());

    connect (ui->closePushButton, &QAbstractButton::clicked, this, &QDialog::close);
}

About::~About ()
{
    delete ui;
}
