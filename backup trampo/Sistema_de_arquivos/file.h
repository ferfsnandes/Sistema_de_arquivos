#ifndef FILE_H
#define FILE_H

#include "filesystemnode.h"

class Directory;

class File : public FileSystemNode {
public:
    File(const QString& name, int size, Directory* parent = nullptr)
        : FileSystemNode(parent), fileName(name), fileSize(size) {}

    QString name() const override { return fileName; }
    bool isDirectory() const override { return false; }
    int size() const { return fileSize; }
    void setSize(int newSize) { fileSize = newSize; }

private:
    QString fileName;
    int fileSize;
};

#endif // FILE_H
