#include <QCoreApplication>
#include <QMutex>
#include <QTimer>
#include <QSharedMemory>
#include <QDebug>
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

        qDebug() << res;
    });
    timer.start();

    return a.exec();
}
