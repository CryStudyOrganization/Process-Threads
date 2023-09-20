#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFile>
#include <QSharedMemory>
#include <QDataStream>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Initilize();

    // Створюємо або відкриваємо memory-mapped файл
    sharedMemory = new QSharedMemory("datafile", this);

    connect(&fileWatcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::fileChanged);
    fileWatcher.addPath(dataPath);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateData);
    timer->start(500);
}

void MainWindow::Initilize()
{
    connect(findChild<QPushButton*>("selectedPathButton"), &QPushButton::clicked, this, &MainWindow::choosePath);
}

void MainWindow::fileChanged(const QString& path)
{
    if (path == dataPath) {
        saveDataToMemoryMappedFile();
    }
}

void MainWindow::updateData()
{
    if (!dataPath.isEmpty()) {
        if (sharedMemory->attach()) {
            QByteArray byteArray;

            qint64 size = sharedMemory->size();

            if (sharedMemory->lock()) {
                byteArray.resize(static_cast<int>(size));
                memcpy(byteArray.data(), sharedMemory->data(), static_cast<size_t>(size));
                sharedMemory->unlock();
            }

            if (size > 0) {
                QString hexData = byteArray.toHex(' ').toUpper();
                ui->textBrowser->setPlainText(hexData);
            } else {
                ui->textBrowser->setPlainText("No data in the memory-mapped file.");
            }
        }
    } else {
        ui->textBrowser->setPlainText("Select a file.");
    }
}


void MainWindow::choosePath()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Виберіть файл", QDir::homePath());

    if (!filePath.isEmpty()) {
        dataPath = filePath;
        ui->currPath->setText(filePath);
        ui->textBrowser->setPlainText("Вибраний шлях: " + filePath);
        saveDataToMemoryMappedFile();
    }
}


void MainWindow::saveDataToMemoryMappedFile()
{
    if (sharedMemory->isAttached()) {
        sharedMemory->detach();
    }

    QFile file(dataPath);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray byteArray = file.readAll();
        file.close();

        if (sharedMemory->create(byteArray.size())) {
            if (sharedMemory->lock()) {
                char *to = static_cast<char*>(sharedMemory->data());
                const char *from = byteArray.constData();
                int size = byteArray.size();
                memcpy(to, from, size);
                sharedMemory->unlock();
            }
        }
    }
}

MainWindow::~MainWindow()
{
    if (sharedMemory->isAttached()) {
        sharedMemory->detach();
    }

    delete ui;
}
