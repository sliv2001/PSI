#include <QTest>
#include "test_mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

Test_mainwindow::Test_mainwindow(QObject *parent) : QObject(parent)
{

}

void Test_mainwindow::test_getYear(){
    QCOMPARE(MainWindow::getYear("1.JPG"), 2021);
    QCOMPARE(MainWindow::getYear("2.jpg"), 0);
}
