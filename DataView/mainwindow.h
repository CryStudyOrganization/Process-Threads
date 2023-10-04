// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFile>
#include <QIODevice>
#include <QSharedMemory>
#include <QMutex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateData();

private:
    Ui::MainWindow *ui;

    QSharedMemory sharedMemory;
    QMutex mutex;
};

#endif // MAINWINDOW_H
