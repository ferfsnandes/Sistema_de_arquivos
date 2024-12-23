#include "file.h"
#include <QDebug>

File::File(const QString& name, int size)
    : fileName(name), fileSize(size) {}

QString File::name() const {
    return fileName;
}

bool File::isDirectory() const {
    return false;
}

int File::size() const {
    return fileSize;
}

void File::setSize(int newSize) {
    if (newSize >= 0) {
        fileSize = newSize;
        qDebug() << "Emitting sizeChanged for" << fileName << "with size" << fileSize;
        emit sizeChanged(fileName, fileSize);
    }
}
