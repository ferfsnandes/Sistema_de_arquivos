#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "filesystemnode.h"
#include <QList>

class Directory : public FileSystemNode {
public:
    Directory(const QString& name);
    QString name() const override;
    bool isDirectory() const override;

    void addNode(FileSystemNode* node);
    void removeNode(FileSystemNode* node);
    QList<FileSystemNode*> children() const;

private:
    QString dirName;
    QList<FileSystemNode*> nodes;
};

#endif // DIRECTORY_H
