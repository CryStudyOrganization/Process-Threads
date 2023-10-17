// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QFile>
#include <QMutex>
#include <QSharedMemory>
#include <QTextBrowser>
#include <QPushButton>
#include <QTextStream>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void Initialize();

    void createData();
    void deleteData();
    void clearData();
    void shuffleData();
    void sortData();

    void updateSharedMemoryData();

private:
    Ui::MainWindow *ui;

    QVector<int> dataVector;

    QPushButton *createButton;
    QPushButton *deleteButton;
    QPushButton *clearButton;
    QPushButton *shuffleButton;
    QPushButton *sortButton;

    QMutex *mutex;
    QSharedMemory sharedMemory;
};

#endif // MAINWINDOW_H
