#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow (QWidget* parent = nullptr);
    ~MainWindow ();

    void updateRecentFiles (const QString& path);

private:
    Ui::MainWindow* ui;
    QString m_file;
    QStringList m_recentFiles;
    void load (const QString& path, bool loadingRecent);
    void updateRecentFiles ();
};
#endif  // MAINWINDOW_H
