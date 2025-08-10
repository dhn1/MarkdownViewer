#include "About.h"

#include <QProcess>
#include <QStandardPaths>
#include <QtGlobal>

#include "LibUiVersion.h"
#include "ui_About.h"


About::About (QWidget* parent) : QDialog (parent), ui (new Ui::About)
{
    ui->setupUi (this);
    ui->versionLabel->setText ("Markdown Viewer: " + qApp->applicationVersion () + "\nlibui: " + libUiVersion () + QStringLiteral ("\nQt: %1.%2.%3").arg (QT_VERSION_MAJOR).arg (QT_VERSION_MINOR).arg (QT_VERSION_PATCH));

    connect (ui->closePushButton, &QAbstractButton::clicked, this, &QDialog::close);

    ui->icon->setPixmap (QPixmap (":/assets/MarkDown.png"));
}

About::~About ()
{
    delete ui;
}
