#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Initilize();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateData);
    timer->start(500);

}

void MainWindow::Initilize(){
    dataFileMutex = new QMutex;
    connect(findChild<QPushButton*>("selectedPathButton"), &QPushButton::clicked, this, &MainWindow::choosePath);
}

void MainWindow::updateData()
{
    if (!QFile::exists(dataPath)) {
        ui->textBrowser->setPlainText(dataPath);
        return;
    }

    QFile file(dataPath);
    if (!file.open(QIODevice::ReadOnly)) {
        ui->textBrowser->setPlainText("Помилка відкриття файлу!");
        return;
    }

    QStringList dataList;
    if (readDataFromFile(file, dataList)) {
        displayData(dataList);
    }

    file.close(); // Обязательное закрытие файла
}

bool MainWindow::readDataFromFile(QFile& file, QStringList& dataList)
{
    // Используем мьютекс для синхронизации доступа к файлу
    dataFileMutex->lock();

    QTextStream stream(&file);

    while (!stream.atEnd()) {
        QString line;
        stream >> line;
        dataList.append(line);
    }

    // Разблокируем мьютекс после завершения операций с файлом
    dataFileMutex->unlock();

    return !dataList.isEmpty();
}

void MainWindow::displayData(const QStringList& dataList)
{
    if (dataList.isEmpty()) {
        ui->textBrowser->setPlainText("Файл пустий.");
    } else {
        ui->textBrowser->setPlainText(dataList.join(" "));
    }
}

void MainWindow::choosePath()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::homePath());

    if (!filePath.isEmpty()) {
        dataPath = filePath;
        ui->currPath->setText(filePath);
        ui->textBrowser->setPlainText("Выбранный путь: " + filePath);
    }
}

MainWindow::~MainWindow()
{
    delete dataFileMutex;
    delete ui;
}
