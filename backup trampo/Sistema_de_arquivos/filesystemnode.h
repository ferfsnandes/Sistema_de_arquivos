#ifndef FILESYSTEMNODE_H
#define FILESYSTEMNODE_H


#include "filesystemmodel.h"
#include "file.h"
#include <QString>

class Directory;  // Declaração antecipada de Directory

class FileSystemNode {
public:
    explicit FileSystemNode(Directory* parent = nullptr) : parentDir(parent) {}
    virtual ~FileSystemNode() {}

    virtual QString name() const = 0;
    virtual bool isDirectory() const = 0;

    Directory* parent() const { return parentDir; }

protected:
    Directory* parentDir;
};

FileSystemModel::FileSystemModel(Directory* root, QObject* parent)
    : QAbstractItemModel(parent), rootDir(root) {}

QModelIndex FileSystemModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Directory* parentDir = !parent.isValid() ? rootDir : static_cast<Directory*>(parent.internalPointer());
    FileSystemNode* childNode = parentDir->children().at(row);

    return createIndex(row, column, childNode);
}

QModelIndex FileSystemModel::parent(const QModelIndex &index) const {
    if (!index.isValid())
        return QModelIndex();

    FileSystemNode* childNode = static_cast<FileSystemNode*>(index.internalPointer());
    Directory* parentDir = childNode->parent();

    if (parentDir == rootDir || !parentDir)
        return QModelIndex();

    int row = parentDir->parent()->children().indexOf(parentDir);
    return createIndex(row, 0, parentDir);
}

#endif // FILESYSTEMNODE_H
