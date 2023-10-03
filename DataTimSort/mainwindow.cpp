// mainwindow.cpp
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
  
    connect(createButton, SIGNAL(clicked()), this, SLOT(createData()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteData()));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearData()));
    connect(shuffleButton, SIGNAL(clicked()), this, SLOT(shuffleData()));
    connect(sortButton, SIGNAL(clicked()), this, SLOT(sortData()));

    // Ініціалізація м'ютексу
    mutex = new QMutex(QMutex::Recursive);

    // Відкриваємо Memory Mapped File (або створюємо новий, якщо він не існує)
    sharedMemory.setKey("DataMemory");
    if (!sharedMemory.create(sizeof(int) * dataVector.size())) {
        if (!sharedMemory.attach()) {
            ui->textBrowser->setPlainText("Помилка при доступі до Memory Mapped File.");
        }
    }
    // Завантаження даних з Memory Mapped File
    updateSharedMemoryData();
}

// Оголосіть цей метод у визначенні класу MainWindow
void MainWindow::updateSharedMemoryData() {
    if (sharedMemory.isAttached()) {
        int *data = static_cast<int *>(sharedMemory.data());
        for (int i = 0; i < dataVector.size(); ++i) {
            data[i] = dataVector[i];
        }
    }
}

void MainWindow::createData() {
    QMutexLocker locker(mutex); // Захоплюємо м'ютекс перед зміною даних
    dataVector.clear();
    for (int i = 0; i < 20; ++i) {
        int number = QRandomGenerator::global()->bounded(91) + 10;
        dataVector.append(number);
    }

    updateSharedMemoryData();

    if (isFile->isChecked()) {
        saveDataToFile();
    }

    updateTextBrowser();
}

void MainWindow::deleteData() {
    QMutexLocker locker(mutex);
    dataVector.clear();

    updateSharedMemoryData(); // Оновлюємо дані в Memory Mapped File

    if (isFile->isChecked()) {
        saveDataToFile();
    }

    updateTextBrowser();
}

void MainWindow::clearData() {
    QMutexLocker locker(mutex);
    dataVector.clear();

    updateSharedMemoryData(); // Оновлюємо дані в Memory Mapped File

    if (isFile->isChecked()) {
        saveDataToFile();
    }
    updateTextBrowser();
}

void MainWindow::shuffleData() {
    QMutexLocker locker(mutex);
    std::random_shuffle(dataVector.begin(), dataVector.end());

    updateSharedMemoryData(); // Оновлюємо дані в Memory Mapped File

    if (isFile->isChecked()) {
        saveDataToFile();
    }
    updateTextBrowser();
}

void MainWindow::sortData() {
    QMutexLocker locker(mutex);
    timsort(dataVector.begin(), dataVector.end());

    updateSharedMemoryData(); // Оновлюємо дані в Memory Mapped File

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
    sharedMemory.detach(); // Відключаємо Memory Mapped File
    delete mutex; // Вивільняємо ресурси м'ютексу
}
