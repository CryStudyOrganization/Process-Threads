#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "timsort.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

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
  
    connect(createButton, SIGNAL(clicked()), this, SLOT(createData()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteData()));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearData()));
    connect(shuffleButton, SIGNAL(clicked()), this, SLOT(shuffleData()));
    connect(sortButton, SIGNAL(clicked()), this, SLOT(sortData()));

    mutex = new QMutex();

    sharedMemory.setKey("DataMemory");

    try {
        if (!sharedMemory.attach()) {
            if (!sharedMemory.create(sizeof(int) * 20)) {
                throw std::runtime_error("Failed to create or attach to Memory Mapped File");
            }
        }
        updateSharedMemoryData();

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::updateSharedMemoryData() {
    if (sharedMemory.isAttached()) {
        int *data = static_cast<int *>(sharedMemory.data());
        for (int i = 0; i < dataVector.size(); ++i) {
            data[i] = dataVector[i];
        }
    }
}

void MainWindow::createData() {
    QMutexLocker locker(mutex);
    dataVector.clear();
    for (int i = 0; i < 20; ++i) {
        int number = QRandomGenerator::global()->bounded(91) + 10;
        dataVector.append(number);
    }

    try {
        updateSharedMemoryData();

        if (isFile->isChecked()) {
            saveDataToFile();
        }

        updateTextBrowser();
    } catch (...) {
        ui->textBrowser->setPlainText("Failed with work to Memory Mapped File: ");
    }
}

void MainWindow::deleteData() {
    QMutexLocker locker(mutex);
    dataVector.clear();

    updateSharedMemoryData();

    if (isFile->isChecked()) {
        saveDataToFile();
    }

    updateTextBrowser();
}

void MainWindow::clearData() {
    QMutexLocker locker(mutex);
    dataVector.clear();

    updateSharedMemoryData();

    if (isFile->isChecked()) {
        saveDataToFile();
    }
    updateTextBrowser();
}

void MainWindow::shuffleData() {
    QMutexLocker locker(mutex);
    std::random_shuffle(dataVector.begin(), dataVector.end());

    updateSharedMemoryData();

    if (isFile->isChecked()) {
        saveDataToFile();
    }
    updateTextBrowser();
}

void MainWindow::sortData() {
    QMutexLocker locker(mutex);
    timsort(dataVector.begin(), dataVector.end());

    updateSharedMemoryData();

    if (isFile->isChecked()) {
        saveDataToFile();
    }
  
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

void MainWindow::saveDataToFile() {
    QFile file(dataPath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        const char *separator = file.exists() ? " " : "";

        for (int i = 0; i < dataVector.size(); ++i) {
            stream << separator << dataVector[i];
            separator = " ";
        }

        file.close();
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
    sharedMemory.detach();
    delete mutex;
}
