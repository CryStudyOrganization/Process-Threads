#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "timsort.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),  ui(new Ui::MainWindow),  fileMutex(new QMutex)
{
    ui->setupUi(this);
    Initilize();
}

void MainWindow::Initilize() {
    isFile = findChild<QCheckBox*>("_isFile");

    connect(findChild<QPushButton*>("selectedPathButton"), &QPushButton::clicked, this, &MainWindow::choosePath);
    connect(findChild<QPushButton*>("_create"), &QPushButton::clicked, this, &MainWindow::createData);
    connect(findChild<QPushButton*>("_delete"), &QPushButton::clicked, this, &MainWindow::deleteData);
    connect(findChild<QPushButton*>("_clear"), &QPushButton::clicked, this, &MainWindow::clearData);
    connect(findChild<QPushButton*>("_shuffle"), &QPushButton::clicked, this, &MainWindow::shuffleData);
    connect(findChild<QPushButton*>("sort"), &QPushButton::clicked, this, &MainWindow::sortData);
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

    if (isFile->isChecked()) {
        saveDataToFile();
    }
}

void MainWindow::saveDataToFile()
{
    // Заблокируем мьютекс для синхронизации доступа к файлу
    QMutexLocker locker(fileMutex);

    QFile file(dataPath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        for (int i = 0; i < dataVector.size(); ++i) {
            if (i > 0) {
                stream << " ";
            }
            stream << dataVector[i];
        }

        file.close();
    }
}

void MainWindow::loadDataFromFile()
{
    // Проверяем наличие файла перед чтением
    if (!QFile::exists(dataPath)) {
        // Обработка отсутствия файла
        return;
    }

    // Заблокируем мьютекс для синхронизации доступа к файлу
    QMutexLocker locker(fileMutex);

    QFile file(dataPath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        QString line = stream.readLine();
        QStringList dataList = line.split(" ", Qt::SkipEmptyParts);

        dataVector.clear();
        for (const QString &str : dataList) {
            bool ok;
            int number = str.toInt(&ok);
            if (ok) {
                dataVector.append(number);
            }
        }

        file.close();
    }
}

MainWindow::~MainWindow()
{
    // Сохраняем данные в файл при закрытии приложения
    if (isFile->isChecked()) {
        saveDataToFile();
    }

    // Удаляем мьютекс
    delete fileMutex;
    delete ui;
}
