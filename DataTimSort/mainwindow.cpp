#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "timsort.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createButton = findChild<QPushButton*>("_create");
    deleteButton = findChild<QPushButton*>("_delete");
    clearButton = findChild<QPushButton*>("_clear");
    shuffleButton = findChild<QPushButton*>("_shuffle");
    sortButton = findChild<QPushButton*>("sort");

    isFile = findChild<QCheckBox*>("_isFile");

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

void MainWindow::updateTextBrowser() {
    QString dataString;
    for (int i = 0; i < dataVector.size(); ++i) {
        dataString += QString::number(dataVector[i]) + " ";
    }

    ui->textBrowser->setPlainText(dataString);
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
}

MainWindow::~MainWindow()
{
    delete ui;
    sharedMemory.detach();
    delete mutex;
}
