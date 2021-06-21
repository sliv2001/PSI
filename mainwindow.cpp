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
    if (photoPath==""){
        qWarning("Empty or incomplete string input");
        return -1;
    }
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(photoPath.toStdString());
    assert(image.get() != 0);
    image->readMetadata();

    Exiv2::ExifData &exifData = image->exifData();
    if (exifData.empty()) {
        qWarning(": No Exif data found in the file");
        return -1;
    }
    Exiv2::Exifdatum& tag = exifData["Exif.Photo.DateTimeOriginal"];
    QString yearStr = QString::fromStdString(tag.toString());
    if (yearStr.length()<4){
        qWarning("Wrong date");
        return -1;
    }

    bool ok;
    int year = yearStr.left(4).toInt(&ok);
    if (!ok||year<1500||year>2500){
        qWarning("Wrong date");
        return -1;
    }

    return year;
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

