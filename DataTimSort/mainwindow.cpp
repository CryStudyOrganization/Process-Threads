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
}

void MainWindow::createData() {
    dataVector.clear();
    for (int i = 0; i < 20; ++i) {
        int number = QRandomGenerator::global()->bounded(91) + 10;
        dataVector.append(number);
    }

    if (isFile->isChecked()) {
        saveDataToFile();
    }

    updateTextBrowser();
}

void MainWindow::deleteData() {
    dataVector.clear();
    if (isFile->isChecked()) {
        saveDataToFile();
    }
    updateTextBrowser();
}

void MainWindow::clearData() {
    dataVector.clear();
    if (isFile->isChecked()) {
        saveDataToFile();
    }
    updateTextBrowser();
}

void MainWindow::shuffleData() {
    std::random_shuffle(dataVector.begin(), dataVector.end());
    if (isFile->isChecked()) {
        saveDataToFile();
    }
    updateTextBrowser();
}

void MainWindow::sortData() {
    timsort(dataVector.begin(), dataVector.end());
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

    // Проверяем, существует ли файл
    bool fileExists = file.exists();

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        // Если файл не существовал, добавляем данные
        if (!fileExists) {
            for (int i = 0; i < dataVector.size(); ++i) {
                stream << dataVector[i] << " ";
            }
        } else {
            // Если файл существовал, перезаписываем данные
            for (int i = 0; i < dataVector.size(); ++i) {
                if (i > 0) {
                    stream << " ";
                }
                stream << dataVector[i];
            }
        }

        file.close();
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
