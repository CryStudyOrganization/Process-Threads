#include <QCoreApplication>
#include <QDebug>
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

        if (!fileManager.openFile(QIODevice::WriteOnly)) {
            qDebug() << "Ошибка открытия файла!";
            return;
        }

        qDebug() << "Файл создан и заполнен случайными числами.";

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

    return a.exec();
}
