#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    context = new TContext();
    ui->tabWidget->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::getYear(QString photoPath){
    if (!QFile(photoPath).exists()){
        qWarning("File %s does not exist", photoPath.toStdString().c_str());
        return -1;
    }
    QString date;
    try {
        Exiv2::Image::AutoPtr image;
        std::string s = photoPath.toStdString();
        image = Exiv2::ImageFactory::open(s, false);
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
#ifndef TESTING
        qWarning("Couldnot get year of photo %s", photoPath.toStdString().c_str());
#endif
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
    Q_UNUSED(objectName);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index);
}


void MainWindow::on_action_2_triggered()
{
    /*Если нажата "Получить из папки", это выполняется */
    QString strdir = QFileDialog::getExistingDirectory(this, "Выбор папки для импорта", QDir::homePath());

    QDir dir;
    if (dir.exists(strdir)){
        delete context;
        context = new TContext();
        context->init(strdir);
    }

}


void MainWindow::on_action_triggered()
{

}

