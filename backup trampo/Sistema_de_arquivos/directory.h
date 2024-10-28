#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "filesystemnode.h"
#include <QList>

class FileSystemNode;  // Declaração antecipada para evitar inclusões circulares

class Directory : public FileSystemNode {
public:
    Directory(const QString& name, Directory* parent = nullptr)
        : FileSystemNode(parent), dirName(name) {}

    QString name() const override { return dirName; }
    bool isDirectory() const override { return true; }

    void addNode(FileSystemNode* node) { nodes.push_back(node); }
    void removeNode(FileSystemNode* node) { nodes.removeOne(node); }
    QList<FileSystemNode*> children() const { return nodes; }

private:
    QString dirName;
    QList<FileSystemNode*> nodes;
};

#endif // DIRECTORY_H
