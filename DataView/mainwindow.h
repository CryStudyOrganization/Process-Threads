#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include <QPushButton>
#include <QTextBrowser>

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

private:
    QString dataPath;
    Ui::MainWindow *ui;
    QPushButton *selectPathButton;
    QTextBrowser *textBrowser;
    QTextBrowser *currPath;
    QMutex *dataFileMutex;

    void updateData();
    bool readDataFromFile(QFile& file, QStringList& dataList);
    void displayData(const QStringList& dataList);

    Q_DISABLE_COPY(MainWindow);
    Q_DISABLE_MOVE(MainWindow);

    void Initilize();

};

#endif // MAINWINDOW_H
