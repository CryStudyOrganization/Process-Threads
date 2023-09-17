#ifndef DATAFILEMANAGER_H
#define DATAFILEMANAGER_H

#include <QString>
#include <QFile>
#include <QVector>

class DataFileManager {
public:
    DataFileManager(const QString& fileName);
    ~DataFileManager();

    bool openFile(QIODevice::OpenMode mode);
    bool fileExists() const;
    bool isEmpty() const;
    bool createAndFillFile(int size);

    void closeFile();
    void writeDataToFile(const QVector<int>& data);

    QVector<int> readDataFromFile();

private:
    QFile file_;
    QString fileName_;
};

#endif // DATAFILEMANAGER_H
