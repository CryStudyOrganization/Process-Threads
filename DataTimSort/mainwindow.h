#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include <QString>
#include <QCheckBox>
#include <QRandomGenerator>
#include <QMutex>
#include <QTextBrowser>
#include <QFile>
#include <QTextStream>
#include <QMutexLocker>
#include <QFileDialog>
#include <QDir>

class QCheckBox;
class QPushButton;
class QTextBrowser;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public Q_SLOTS:
    void createData();
    void deleteData();
    void clearData();
    void shuffleData();
    void sortData();
    void updateTextBrowser();

private Q_SLOTS:
    void choosePath();
    void saveDataToMemoryMappedFile();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString dataPath;
    QVector<int> dataVector;

    QPushButton *selectPathButton;
    QTextBrowser *currPath;

    QPushButton *createButton;
    QPushButton *deleteButton;
    QPushButton *clearButton;
    QPushButton *shuffleButton;
    QPushButton *sortButton;

    Ui::MainWindow *ui;

    std::unique_ptr<QMutex> mutex;

    Q_DISABLE_COPY(MainWindow);
    Q_DISABLE_MOVE(MainWindow);

    void Initilize();
};

#endif // MAINWINDOW_H
