#ifndef FILE_H
#define FILE_H

#include "filesystemnode.h"

class File : public FileSystemNode {
public:
    File(const QString& name, int size);
    QString name() const override;
    bool isDirectory() const override;
    int size() const;
    void setSize(int newSize);

private:
    QString fileName;
    int fileSize;
};

#endif // FILE_H
