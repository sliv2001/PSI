#include <QTest>
#include "test_mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

Test_mainwindow::Test_mainwindow(QObject *parent) : QObject(parent)
{

}

void Test_mainwindow::test_setupTabView(){
    MainWindow w;
    int i;
    for (i=0; i<5; i++){
        w.setupTabView(i, i);
        QCOMPARE(w.ui->tabWidget->tabText(i), QString::number(i));
    }
    QCOMPARE(w.ui->tabWidget->count(), 5);
}
