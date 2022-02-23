#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QProgressBar>

#include "exiv2/exiv2.hpp"

#include "tcontext.h"
#include "ttabledelegate.h"

//#define TESTING

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow *ui;
    TContext* context=NULL;
    QProgressBar* bar;

    static int getYear(QString photoPath);
    void deleteAllWidgets();

private slots:

    void drawContext();

    void startScanningFilesystem();

    void startExport();

    void finishScanningFilesystem();

    void finishExport();

    void progressAtWork(int progress);

    void setupBar(int min, int max);

    void on_tabWidget_objectNameChanged(const QString &objectName);

    void on_tabWidget_currentChanged(int index);

    void on_action_2_triggered();

    void on_action_triggered();

    void on_action_3_triggered();

private:

    void lockInterface(bool lock);
};
#endif // MAINWINDOW_H
