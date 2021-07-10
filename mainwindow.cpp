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
        Exiv2::Image::UniquePtr image;
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

void MainWindow::drawContext()
{
    deleteAllWidgets();

    for (int i = 0; i<context->tabCount(); i++){
        QWidget* w = new QWidget();
        ui->tabWidget->addTab(w, context->tabByIndex(i)->name);

        QVBoxLayout* layout = new QVBoxLayout(w);
        QTableView* table = new QTableView(w);
        TTableDelegate* delegate = new TTableDelegate();
        layout->addWidget(table);
        table->setModel(context->tabByIndex(i));
        table->setItemDelegate(delegate);
        table->show();
        table->resizeColumnsToContents();
        table->resizeRowsToContents();
    }
    ui->tabWidget->setVisible(true);
}

void MainWindow::deleteAllWidgets()
{
    for (int i=ui->tabWidget->count()-1; i>=0; i--){
        QWidget* w = ui->tabWidget->widget(i);
        ui->tabWidget->removeTab(i);
        delete w;
    }
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
        drawContext();
    }

}


void MainWindow::on_action_triggered()
{

}

