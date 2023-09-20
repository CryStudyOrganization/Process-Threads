#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include <QPushButton>
#include <QTextBrowser>
#include <QFile>
#include <QFileDialog>
#include <QTimer>
#include <QSharedMemory>
#include <QFileSystemWatcher>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private Q_SLOTS:
    void choosePath();
    void saveDataToMemoryMappedFile();
    void fileChanged(const QString& path);

private:
    QString dataPath;
    Ui::MainWindow *ui;

    QPushButton *selectPathButton;
    QTextBrowser *textBrowser;
    QTextBrowser *currPath;

    QMutex *dataFileMutex;
    QSharedMemory *sharedMemory;
    QFileSystemWatcher fileWatcher;

    void Initilize();
    void updateData();
    bool readDataFromFile(QFile& file, QStringList& dataList);
    void displayData(const QStringList& dataList);

    Q_DISABLE_COPY(MainWindow);
    Q_DISABLE_MOVE(MainWindow);
};

#endif // MAINWINDOW_H
