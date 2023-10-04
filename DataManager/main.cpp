#include <QCoreApplication>
#include <QMutex>
#include <QTimer>
#include <QSharedMemory>
#include <QDebug>
<<<<<<< HEAD
#include <QThread>
#include <algorithm>

const int DataSize = 20;
const int DelayMilliseconds = 500;

// Функция обратной сортировки
bool compareIntsReverse(const qint32& a, const qint32& b) {
    return a > b;
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QSharedMemory sharedMemory;
    sharedMemory.setKey("DataMemory");

    if (!sharedMemory.attach()) {
        qCritical() << "Shared memory does not exist. Run the data generation program first.";
        return 1;
    }

    qint32* data = reinterpret_cast<qint32*>(sharedMemory.data());

    QMutex mutex;

    QTimer timer;

    QObject::connect(&timer, &QTimer::timeout, [&](){
        QString res = "Iteration:";
        {
            QMutexLocker locker(&mutex);
=======
#include <QRandomGenerator>
#include <QCoreApplication>
#include <QMutex>
#include "datafilemanager.h"

class MyApplication : public QCoreApplication {
public:
    MyApplication(int &argc, char **argv) : QCoreApplication(argc, argv) {}

    void runProgram() {
        const QString fileName = "../datafiles/data.dat";
        DataFileManager fileManager(fileName);
>>>>>>> 7e7827db43ba98fa92285c677958650d776a0f6e

            bool swapped = false;

            for (int i = 1; i < DataSize; ++i) {
                if (data[i - 1] > data[i]) {
                    qint32 temp = data[i - 1];
                    data[i - 1] = data[i];
                    data[i] = temp;
                    swapped = true;
                }
            }

            if (!swapped) {
                timer.stop();
            }

            for (int i = 0; i < DataSize; ++i) {
                if (i != 0) {
                    res.append(" ");
                }
                res.append(QString::number(data[i]));
            }
        }

        QThread::msleep(DelayMilliseconds);

<<<<<<< HEAD
        qDebug() << res;
    });
    timer.start();
=======
        QVector<int> data = generateRandomData();
        writeDataToFile(fileManager, data);

        qDebug() << "Файл содержит следующие данные до сортировки:";
        displayData(data);

        sortData(data);
        writeDataToFile(fileManager, data);

        qDebug() << "Файл содержит следующие данные после сортировки:";
        displayData(data);

        qDebug() << "Работа завершена.";
    }

    ~MyApplication(){

        delete mutex;
    }

private:
    QVector<int> generateRandomData() {
        QVector<int> data{};
        for (int i = 0; i < 20; ++i) {
            int number = QRandomGenerator::global()->bounded(91) + 10;
            data.append(number);
        }
        return data;
    }

    void writeDataToFile(DataFileManager& fileManager, const QVector<int>& data) {
        QMutexLocker locker(mutex); // Автоматический захват и освобождение мьютекса
        fileManager.writeDataToFile(data);
    }

    void displayData(const QVector<int>& data) {
        QString result;
        for (int number : data) {
            result += QString::number(number) + " ";
        }
        qDebug() << result.trimmed();
    }

    void sortData(QVector<int>& data) {
        std::sort(data.begin(), data.end());
    }

    QMutex *mutex; // Мьютекс для синхронизации доступа к файлу
};

int main(int argc, char *argv[])
{
    MyApplication a(argc, argv);

    a.runProgram();
>>>>>>> 7e7827db43ba98fa92285c677958650d776a0f6e

    return a.exec();
}
