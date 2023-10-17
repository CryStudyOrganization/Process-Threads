#include <QCoreApplication>
#include <QMutex>
#include <QTimer>
#include <QSharedMemory>
#include <QDebug>
#include <QThread>
#include <QMutexLocker>
#include <algorithm>

const int DataSize = 20;
const int DelayMilliseconds = 500;

// Функция обратной сортировки
bool compareIntsReverse(const qint32& a, const qint32& b) {
    return a > b;
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QSharedMemory sharedMemory("DataMemory");

    if (!sharedMemory.attach()) {
        qCritical() << "Shared memory does not exist. Run the data generation program first.";
        return 1;
    }

    qint32* data = reinterpret_cast<qint32*>(sharedMemory.data());

    QMutex mutex; // Объект мьютекса для синхронизации доступа к разделяемой памяти

    QTimer timer;

    QObject::connect(&timer, &QTimer::timeout, [&](){
        QString res = "Iteration:";
        bool swapped = false;

        {
            QMutexLocker locker(&mutex); // Блокируем мьютекс при доступе к данным

            for (int i = 1; i < DataSize; ++i) {
                if (data[i - 1] > data[i]) {
                    std::swap(data[i - 1], data[i]);
                    swapped = true;
                }
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

        QThread::msleep(DelayMilliseconds);

        qDebug() << res;
    });

    timer.start();

    return a.exec();
}
