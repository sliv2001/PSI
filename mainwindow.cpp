#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tabWidget->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tabWidget_objectNameChanged(const QString &objectName)
{

}

void MainWindow::setupTabView(int index, int year){
    QTableWidget *newTable = new QTableWidget();
    if (ui->tabWidget->count()-1<index){
        ui->tabWidget->addTab(newTable, QString::number(year));
        index=ui->tabWidget->count()-1;
    }
    else{
        newTable->setParent(ui->tabWidget->widget(index));
    }
    QVBoxLayout *layout = new QVBoxLayout;
    ui->tabWidget->setLayout(layout);
    newTable->show();
}
