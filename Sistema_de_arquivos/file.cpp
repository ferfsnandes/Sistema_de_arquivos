#include "file.h"

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
    fileSize = newSize;
}
