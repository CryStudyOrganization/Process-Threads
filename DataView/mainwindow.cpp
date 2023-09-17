#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создание таймера
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateData);
    timer->start(500); // Таймер срабатывает каждые 500 миллисекунд
}

void MainWindow::updateData()
{
    // Формирование пути к файлу с данными
    const QString dataFilePath = "../datafiles/data.dat";

    // Проектирование файла в память
    QFile file(dataFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        ui->textBrowser->setPlainText("Ошибка открытия файла!");
        return;
    }

    QTextStream stream(&file);
    QStringList dataList;

    // Чтение данных из файла и формирование списка
    while (!stream.atEnd()) {
        QString line;
        stream >> line;
        dataList.append(line);
    }

    if (dataList.isEmpty()) {
        // Если список данных пустой, то выводим сообщение об этом
        ui->textBrowser->setPlainText("Файл пуст.");
    } else {
        // Иначе выводим данные из файла
        ui->textBrowser->setPlainText(dataList.join(" "));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
