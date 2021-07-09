#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QFileDialog>
#include <QFileSystemModel>
#include "exiv2/exiv2.hpp"
#include "tcontext.h"

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
    TContext* context;
    static int getYear(QString photoPath);

private slots:
    void on_tabWidget_objectNameChanged(const QString &objectName);

    void on_tabWidget_currentChanged(int index);

    void on_action_2_triggered();

    void on_action_triggered();

private:

};
#endif // MAINWINDOW_H
