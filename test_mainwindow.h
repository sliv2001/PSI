#ifndef TEST_MAINWINDOW_H
#define TEST_MAINWINDOW_H

#include <QObject>

class Test_mainwindow : public QObject
{
    Q_OBJECT
public:
    explicit Test_mainwindow(QObject *parent = nullptr);

private slots:
    void test_setupTabView();
    void test_getYear();
signals:

};

#endif // TEST_MAINWINDOW_H
