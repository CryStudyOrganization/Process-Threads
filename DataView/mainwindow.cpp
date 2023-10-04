#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QThread>

#include <QFile>
#include <QSharedMemory>
#include <QDataStream>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sharedMemory.setKey("DataMemory");

    try {
        if (!sharedMemory.attach()) {
            if (!sharedMemory.create(sizeof(int) * 20)) {
                throw std::runtime_error("Ошибка при создании Memory Mapped File.");
            }
        }
    } catch (const std::exception& ex) {
        ui->textBrowser->setPlainText(QString("Ошибка: %1").arg(ex.what()));
    }

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateData);
    timer->start(500);
}

void MainWindow::updateData()
{
    QMutexLocker locker(&mutex);

    if (sharedMemory.isAttached()) {
        try {
            int *data = (int *)sharedMemory.data();

            QStringList dataList;

            for (int i = 0; i < 20; ++i) {
                dataList.append(QString::number(data[i]));
            }

<<<<<<< HEAD
            if (dataList.isEmpty()) {
                ui->textBrowser->setPlainText("Дані відсутні.");
            } else {
                ui->textBrowser->setPlainText(dataList.join(" "));
            }
        } catch (const std::exception& ex) {
            ui->textBrowser->setPlainText(QString("Ошибка: %1").arg(ex.what()));
=======
        if (dataList.isEmpty()) {
            ui->textBrowser->setPlainText("Дані відсутні.");
        } else {
            ui->textBrowser->setPlainText(dataList.join(" "));

>>>>>>> 7e7827db43ba98fa92285c677958650d776a0f6e
        }
    }
}

MainWindow::~MainWindow()
{
    sharedMemory.detach();
    delete ui;
}
