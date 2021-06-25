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
    if (!QFile(photoPath).exists()){
        qWarning("File %s does not exist", photoPath.toStdString().c_str());
        return -1;
    }
    QString date;
    try {
        Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(photoPath.toStdString().c_str());
        assert(image.get() != 0);
        image->readMetadata();
        Exiv2::ExifData &data = image->exifData();

        date = QString::fromStdString(data["Exif.Photo.DateTimeOriginal"].toString());
        date = date.left(4);
    }  catch (Exiv2::AnyError &err) {
        qWarning("Couldnot read metadata \"Exif.Photo.DateTimeOriginal\" from file %s.",
                 photoPath.toStdString().c_str());
        return 0;
    }

    bool ok=false;
    int year = date.toInt(&ok);
    if (!ok){
        qWarning("Couldnot get year of photo %s", photoPath.toStdString().c_str());
        return 0;
    }
    if (year<1500||year>2500){
        qWarning("Year of photo %s is out of range", photoPath.toStdString().c_str());
        return 0;
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
    getYear("1.jpg");
}

