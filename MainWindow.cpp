#include "MainWindow.h"

#include <MarkdownImporter.h>

#include <QFileDialog>
#include <QSettings>

#include "./ui_MainWindow.h"
#include "About.h"

MainWindow::MainWindow (QWidget* parent) : QMainWindow (parent), ui (new Ui::MainWindow)
{
    ui->setupUi (this);
    connect (ui->actionOpen, &QAction::triggered, this, [this] (bool) {
        auto res = QFileDialog::getOpenFileName (this, "Open markdown file", m_file);
        if (!res.isEmpty ())
        {
            load (res, false);
        }
    });

    connect (ui->actionReload, &QAction::triggered, this, [this] (bool) {
        load (m_file, true);
    });

    connect (ui->actionClose, &QAction::triggered, this, &MainWindow::close);
    connect (ui->actionAbout, &QAction::triggered, this, [this] {
        About abt (this);
        abt.exec ();
    });


    QSettings settings;

    int const count = settings.beginReadArray (QByteArrayLiteral ("recent"));
    for (int i = 0; i < count; i++)
    {
        settings.setArrayIndex (i);
        m_recentFiles << settings.value (QStringLiteral ("file")).toString ();
    }
    settings.endArray ();
    updateRecentFiles ();

}

void MainWindow::load (const QString& path, bool loadingRecent)
{
    QFile in (path);
    in.open (QFile::ReadOnly);
    if (in.isOpen ())
    {
        if (!loadingRecent)
        {
            updateRecentFiles (path);
        }
        auto data = in.readAll ();

        auto doc = new QTextDocument (ui->textBrowser);

        MarkdownImporter mi (doc, MarkdownImporter::DialectCommonMark | MarkdownImporter::FeatureTables | MarkdownImporter::FeaturePermissiveATXHeaders);

        QFontDatabase::addApplicationFont (QStringLiteral (":/Assets/WorkSans-VariableFont_wght.ttf"));
        auto font = QFont ("Work Sans", 14);
        QTextCharFormat charFmt;
        charFmt.setFont (font);
        mi.setDefaultFormat (QTextBlockFormat (), charFmt);
        for (int level = 0; level < 7; level++)
        {
            auto fmt = mi.heading (level);
            fmt.second.setForeground (QBrush (QColor ::fromRgb (0x00, 0xc0, 0xff)));
            mi.setHeading (level, fmt.first, fmt.second);
        }

        mi.import (data);

        ui->textBrowser->setDocument (doc);
    }
}

MainWindow::~MainWindow ()
{
    delete ui;
}

void MainWindow::updateRecentFiles ()
{
    ui->menuRecent_files->clear ();
    for (const auto& file : std::as_const (m_recentFiles))
    {
        auto action = ui->menuRecent_files->addAction (file);
        connect (action, &QAction::triggered, this, [this, action] {
            load (action->text (), false);
        });
    }
 }

void MainWindow::updateRecentFiles (const QString& path)
{
    if (!path.isEmpty ())
    {
        // Does it exist?
        int i = 0;
        for (; i < m_recentFiles.count (); i++)
        {
            if (m_recentFiles.at (i) == path)
            {
                m_recentFiles.remove (i);
                m_recentFiles.insert (0, path);
                break;
            }
        }
        if (i == m_recentFiles.count ())
        {
            m_recentFiles.insert (0, path);
        }
    }
    while (m_recentFiles.count () > 5)
    {
        m_recentFiles.takeLast ();
    }

    QSettings settings;
    settings.beginWriteArray (QByteArrayLiteral ("recent"));

    ui->menuRecent_files->clear ();
    int idx = 0;
    for (const auto& file : std::as_const (m_recentFiles))
    {
        settings.setArrayIndex (idx++);
        settings.setValue (QStringLiteral ("file"), file);
    }
    settings.endArray ();
    updateRecentFiles ();
}
