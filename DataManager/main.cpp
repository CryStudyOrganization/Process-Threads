#include <QCoreApplication>
#include <QDebug>
#include <QRandomGenerator>
#include <QCoreApplication>

#include "datafilemanager.h"

QString formatData(const QVector<int>& data) {
    QString result{};

    for (int i = 0; i < data.size(); ++i) {
        result += QString::number(data[i]) + " ";
    }

    return result.trimmed();
}

QVector<int> FillBeforeRead(){
    QVector<int> data{};
    for (int i = 0; i < 20; ++i) {
        int number = QRandomGenerator::global()->bounded(91) + 10;
        data.append(number);
    }

    return data;
}

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

        QVector<int> data = FillBeforeRead();
        fileManager.writeDataToFile(data);

        qDebug() << "Файл содержит следующие данные до сортировки:";
        qDebug() << formatData(data);

        std::sort(data.begin(), data.end());
        fileManager.writeDataToFile(data);

        qDebug() << "Файл содержит следующие данные после сортировки:";
        qDebug() << formatData(data);

        qDebug() << "Работа завершена.";
    }
};

int main(int argc, char *argv[])
{
    MyApplication a(argc, argv);

    a.runProgram(); // Запускаем программу в начале

    return a.exec();
}
