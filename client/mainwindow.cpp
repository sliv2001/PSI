#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tabWidget->setVisible(false);

    bar = new QProgressBar(this->statusBar());
    this->statusBar()->addWidget(bar);
    bar->setVisible(false);
    bar->setTextVisible(true);

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
        qWarning("Couldnot get year of photo %s", (const char*)photoPath.toLocal8Bit());
#endif
        return 0;
    }
    if (year<1500||year>2500){
        qWarning("Year of photo %s is out of range", (const char*)photoPath.toLocal8Bit());
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

void MainWindow::lockInterface(bool lock)
{
    ui->centralwidget->setEnabled(!lock);
    ui->menubar->setEnabled(!lock);

    this->bar->setVisible(lock);
}

void MainWindow::startScanningFilesystem()
{
    bar->setFormat("Сканирование: Выполнено %p%; %v / %m");
    lockInterface(true);
}

void MainWindow::startExport()
{
    bar->setFormat("Экспорт: Выполнено %p%; %v / %m");
    lockInterface(true);
}

void MainWindow::finishScanningFilesystem()
{
    lockInterface(false);
    drawContext();
#ifdef NET_RECOGNITION
    context->recognize();
#endif
    delete context->fileSystemScanWatcher;
    bar->setRange(0, 0);
}

void MainWindow::finishExport()
{
    lockInterface(false);
    delete context->exportWatcher;
    bar->setRange(0, 0);
}

void MainWindow::progressAtWork(int progress)
{
    this->bar->setValue(progress);

}

void MainWindow::setupBar(int min, int max)
{
    this->bar->setMinimum(min);
    this->bar->setMaximum(max);
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
        startScanningFilesystem();
        if (context!=NULL)
            delete context;
        context = new TContext();
        context->fileSystemScanWatcher = new QFutureWatcher<void>();
        connect(context->fileSystemScanWatcher, &QFutureWatcher<void>::finished, this, &MainWindow::finishScanningFilesystem);
        connect(context->fileSystemScanWatcher, &QFutureWatcher<void>::progressValueChanged, this, &MainWindow::progressAtWork);
        connect(context->fileSystemScanWatcher, &QFutureWatcher<void>::progressRangeChanged, this, &MainWindow::setupBar);
        QFuture<void> future = context->init(strdir);
        context->fileSystemScanWatcher->setFuture(future);

        setupBar(0, 0);
    }

}


void MainWindow::on_action_triggered()
{

}


void MainWindow::on_action_3_triggered()
{
    QString strdir = QFileDialog::getExistingDirectory(this, "Выбор папки для экспорта", QDir::homePath());

    QDir dir(strdir);
    if (strdir!=""){
        startExport();
        context->exportWatcher = new QFutureWatcher<void>();
        connect(context->exportWatcher, &QFutureWatcher<void>::finished, this, &MainWindow::finishExport);
        connect(context->exportWatcher, &QFutureWatcher<void>::progressValueChanged, this, &MainWindow::progressAtWork);
        connect(context->exportWatcher, &QFutureWatcher<void>::progressRangeChanged, this, &MainWindow::setupBar);
        QFuture<void> future = context->Export(dir);
    }
}

