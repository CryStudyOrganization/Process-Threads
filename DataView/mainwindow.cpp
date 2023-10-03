#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sharedMemory.setKey("DataMemory");

    if (!sharedMemory.create(sizeof(int) * 20)) {
        if (!sharedMemory.attach()) {
            ui->textBrowser->setPlainText("Помилка при доступі до Memory Mapped File.");
        }
    }

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateData);
    timer->start(500);
}

void MainWindow::updateData()
{
    QMutexLocker locker(&mutex);

    if (sharedMemory.isAttached()) {
        int *data = (int *)sharedMemory.data();

        QStringList dataList;

        for (int i = 0; i < 20; ++i) {
            dataList.append(QString::number(data[i]));
        }

        if (dataList.isEmpty()) {
            ui->textBrowser->setPlainText("Дані відсутні.");
        } else {
            ui->textBrowser->setPlainText(dataList.join(" "));
        }
    }
}

MainWindow::~MainWindow()
{
    sharedMemory.detach();
    delete ui;
}
