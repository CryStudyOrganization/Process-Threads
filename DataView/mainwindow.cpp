#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QThread>
#include <QFile>
#include <QSharedMemory>
#include <QDataStream>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), mutex(new QMutex)
{
    ui->setupUi(this);

    sharedMemory.setKey("DataMemory");

    if (!sharedMemory.attach()) {
        if (!sharedMemory.create(sizeof(int) * 20)) {
            ui->textBrowser->setPlainText("Ошибка при создании Memory Mapped File.");
        }
    }

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateData);
    timer->start(500);
}

void MainWindow::updateData()
{
    QMutexLocker locker(mutex);

    if (sharedMemory.isAttached()) {
        int *data = reinterpret_cast<int *>(sharedMemory.data());

        QString dataString;
        for (int i = 0; i < 20; ++i) {
            dataString += QString("*").repeated(data[i]) + " ";
        }

        if (dataString.isEmpty()) {
            ui->textBrowser->setPlainText("Данные отсутствуют.");
        } else {
            ui->textBrowser->setPlainText(dataString);
        }
    }
}

MainWindow::~MainWindow()
{
    delete mutex;
    sharedMemory.detach();
    delete ui;
}
