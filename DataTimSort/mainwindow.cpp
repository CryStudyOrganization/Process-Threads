// mainwindow.cpp
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "timsort.h"

#include <QFile>
#include <QTextStream>

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

    // Ініціалізація м'ютексу
    mutex = new QMutex(QMutex::Recursive);

    // Відкриваємо Memory Mapped File (або створюємо новий, якщо він не існує)
    sharedMemory.setKey("DataMemory");
    if (!sharedMemory.create(sizeof(int) * dataVector.size())) {
        if (!sharedMemory.attach()) {
            ui->textBrowser->setPlainText("Помилка при доступі до Memory Mapped File.");
        }
    }G 7UJM
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
    sharedMemory.detach(); // Відключаємо Memory Mapped File
    delete mutex; // Вивільняємо ресурси м'ютексу
}
