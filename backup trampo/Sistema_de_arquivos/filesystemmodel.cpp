#include "filesystemmodel.h"
#include "file.h"
#include <QDebug>

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

int FileSystemModel::rowCount(const QModelIndex &parent) const {
    if (parent.column() > 0)
        return 0;

    Directory* parentDir = !parent.isValid() ? rootDir : static_cast<Directory*>(parent.internalPointer());
    return parentDir->children().size();
}

int FileSystemModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    FileSystemNode* node = static_cast<FileSystemNode*>(index.internalPointer());
    if (role == Qt::DisplayRole) {
        return node->name();
    }
    return QVariant();
}

void FileSystemModel::addFile(const QString& name, int size) {
    File* newFile = new File(name, size, rootDir);
    beginInsertRows(QModelIndex(), rootDir->children().size(), rootDir->children().size());
    rootDir->addNode(newFile);
    endInsertRows();
}

void FileSystemModel::deleteFile(const QModelIndex& index) {
    if (!index.isValid())
        return;

    FileSystemNode* node = static_cast<FileSystemNode*>(index.internalPointer());
    int row = index.row();

    beginRemoveRows(index.parent(), row, row);
    rootDir->removeNode(node);
    delete node;
    endRemoveRows();
}
