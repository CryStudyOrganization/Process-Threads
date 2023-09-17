#include "datafilemanager.h"
#include <QTextStream>
#include <QRandomGenerator>

DataFileManager::DataFileManager(const QString& fileName) : fileName_(fileName) {
    file_.setFileName(fileName_);
}

DataFileManager::~DataFileManager() {
    closeFile();
}

bool DataFileManager::openFile(QIODevice::OpenMode mode) {
    closeFile();
    return file_.open(mode);
}

void DataFileManager::closeFile() {
    if (file_.isOpen()) {
        file_.close();
    }
}

bool DataFileManager::fileExists() const {
    return file_.exists();
}

bool DataFileManager::isEmpty() const {
    return file_.size() == 0;
}

bool DataFileManager::createAndFillFile(int size) {
    if (!openFile(QIODevice::WriteOnly)) {
        return false;
    }

    QTextStream stream(&file_);
    for (int i = 0; i < size; ++i) {
        int number = QRandomGenerator::global()->bounded(91) + 10;
        stream << number << ' ';
    }

    closeFile();
    return true;
}

QVector<int> DataFileManager::readDataFromFile() {
    QVector<int> data;
    if (!openFile(QIODevice::ReadOnly)) {
        return data;
    }

    QTextStream stream(&file_);
    while (!stream.atEnd()) {
        int number;
        stream >> number;
        data.append(number);
    }

    closeFile();
    return data;
}

void DataFileManager::writeDataToFile(const QVector<int>& data) {
    if (!openFile(QIODevice::WriteOnly)) {
        return;
    }

    QTextStream stream(&file_);
    for (int i = 0; i < data.size(); ++i) {
        stream << data[i] << ' ';
    }

    closeFile();
}
