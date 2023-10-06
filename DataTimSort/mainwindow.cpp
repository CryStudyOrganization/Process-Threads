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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mutex(new QMutex)
{
    ui->setupUi(this);
    Initialize();
}

void MainWindow::Initialize()
{
    connect(ui->_create, &QPushButton::clicked, this, &MainWindow::createData);
    connect(ui->_delete, &QPushButton::clicked, this, &MainWindow::deleteData);
    connect(ui->_clear, &QPushButton::clicked, this, &MainWindow::clearData);
    connect(ui->_shuffle, &QPushButton::clicked, this, &MainWindow::shuffleData);
    connect(ui->sort, &QPushButton::clicked, this, &MainWindow::sortData);

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

void MainWindow::updateSharedMemoryData()
{
    if (sharedMemory.isAttached())
    {
        int *data = static_cast<int *>(sharedMemory.data());
        for (int i = 0; i < dataVector.size(); ++i)
        {
            data[i] = dataVector[i];
        }

        QStringList dataStringList;
        for (int i = 0; i < dataVector.size(); ++i)
        {
            dataStringList.append(QString::number(dataVector[i]));
        }

        ui->textBrowser->setPlainText(dataStringList.join(" "));
    }
}

void MainWindow::createData()
{
    QMutexLocker locker(mutex);
    dataVector.clear();
    for (int i = 0; i < 20; ++i)
    {
        int number = QRandomGenerator::global()->bounded(91) + 10;
        dataVector.append(number);
    }

    updateSharedMemoryData();
}

void MainWindow::deleteData()
{
    QMutexLocker locker(mutex);

    dataVector.clear();
    updateSharedMemoryData();
}

void MainWindow::clearData()
{
    QMutexLocker locker(mutex);

    dataVector.clear();
    updateSharedMemoryData();
}

void MainWindow::shuffleData()
{
    QMutexLocker locker(mutex);

    std::random_shuffle(dataVector.begin(), dataVector.end());
    updateSharedMemoryData();
}

void MainWindow::sortData()
{
    QMutexLocker locker(mutex);

    timsort(dataVector.begin(), dataVector.end());
    updateSharedMemoryData();
}

MainWindow::~MainWindow()
{
    delete ui;
    sharedMemory.detach();
    delete mutex;
}
