#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "timsort.h"

#include <QSharedMemory>
#include <QRandomGenerator>
#include <QDebug>
#include <QMutex>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Initilize();
}

void MainWindow::Initilize()
{
    connect(findChild<QPushButton*>("selectedPathButton"), &QPushButton::clicked, this, &MainWindow::choosePath);
    connect(findChild<QPushButton*>("_create"), &QPushButton::clicked, this, &MainWindow::createData);
    connect(findChild<QPushButton*>("_delete"), &QPushButton::clicked, this, &MainWindow::deleteData);
    connect(findChild<QPushButton*>("_clear"), &QPushButton::clicked, this, &MainWindow::clearData);
    connect(findChild<QPushButton*>("_shuffle"), &QPushButton::clicked, this, &MainWindow::shuffleData);
    connect(findChild<QPushButton*>("sort"), &QPushButton::clicked, this, &MainWindow::sortData);

    // Initialize the mutex using std::unique_ptr
    mutex = std::make_unique<QMutex>();
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

void MainWindow::createData()
{
    dataVector.clear();
    for (int i = 0; i < 20; ++i) {
        int number = QRandomGenerator::global()->bounded(91) + 10;
        dataVector.append(number);
    }

    updateTextBrowser();
}

void MainWindow::deleteData()
{
    dataVector.clear();
    updateTextBrowser();
}

void MainWindow::clearData()
{
    dataVector.clear();
    updateTextBrowser();
}

void MainWindow::shuffleData()
{
    std::random_shuffle(dataVector.begin(), dataVector.end());
    updateTextBrowser();
}

void MainWindow::sortData()
{
    timsort(dataVector.begin(), dataVector.end());
    updateTextBrowser();
}

void MainWindow::updateTextBrowser()
{
    QString dataString;
    for (int i = 0; i < dataVector.size(); ++i) {
        dataString += QString::number(dataVector[i]) + " ";
    }

    ui->textBrowser->setPlainText(dataString);

    saveDataToMemoryMappedFile();
}

void MainWindow::saveDataToMemoryMappedFile()
{
    // Open or create the memory-mapped file
    QSharedMemory sharedMemory("datafile");

    if (!sharedMemory.create(dataVector.size() * sizeof(int))) {
        qDebug() << "Error creating memory-mapped file: " << sharedMemory.errorString();
        return;
    }

    // Get a pointer to the data in the memory-mapped file
    int *sharedData = static_cast<int*>(sharedMemory.data());

    // Lock the mutex before writing to the file
    QMutexLocker locker(mutex.get());

    // Copy data from dataVector to the memory-mapped file
    QByteArray byteArray(reinterpret_cast<char*>(sharedData), dataVector.size() * sizeof(int));
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    for (int i = 0; i < dataVector.size(); ++i) {
        stream << dataVector[i];
    }

    // Unlock the mutex after writing
}


MainWindow::~MainWindow()
{
    delete ui;
}
