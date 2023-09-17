#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include <QString>
#include <QCheckBox>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void createData();
    void deleteData();
    void clearData();
    void shuffleData();
    void sortData();
    void saveDataToFile();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateTextBrowser();

private:
    const QString dataPath = "../datafiles/data.dat";
    QVector<int> dataVector;

    QCheckBox *isFile;

    QPushButton *createButton;
    QPushButton *deleteButton;
    QPushButton *clearButton;
    QPushButton *shuffleButton;
    QPushButton *sortButton;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
