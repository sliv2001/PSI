#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * Эта функция создаёт на index-том табе табвью с этим номером года
    */
    void setupTabView(int index, int year);
    Ui::MainWindow *ui;

private slots:
    void on_tabWidget_objectNameChanged(const QString &objectName);

    void on_tabWidget_currentChanged(int index);

private:

};
#endif // MAINWINDOW_H
