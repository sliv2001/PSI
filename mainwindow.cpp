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

void MainWindow::setupTabView(int index, int year){
    QTableWidget *newTable = new QTableWidget();
    if (ui->tabWidget->count()-1<index){
        ui->tabWidget->addTab(newTable, QString::number(year));
        index=ui->tabWidget->count()-1;
    }
    else{
        newTable->setParent(ui->tabWidget->widget(index));
        ui->tabWidget->setTabText(index, QString::number(year));
    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(newTable);
    ui->tabWidget->widget(index)->setLayout(layout);
    newTable->show();
}

QFileSystemModel* MainWindow::getFilesToModel(QString dir){
    /*TODO сделать эту функцию */
    return NULL;
}

int MainWindow::getYear(QString photoPath){
    return 0;
}

void MainWindow::on_tabWidget_objectNameChanged(const QString &objectName)
{

}

void MainWindow::on_tabWidget_currentChanged(int index)
{

}


void MainWindow::on_action_2_triggered()
{
    /*Если нажата "Получить из папки", это выполняется */
    QString dir = QFileDialog::getExistingDirectory(this, "Выбор папки для импорта", QDir::homePath());
    if (dir!="")
        getFilesToModel(dir);
}

