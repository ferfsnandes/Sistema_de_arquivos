#include "directory.h"

Directory::Directory(const QString& name)
    : dirName(name) {}

QString Directory::name() const {
    return dirName;
}

bool Directory::isDirectory() const {
    return true;
}

void Directory::addNode(FileSystemNode* node) {
    nodes.push_back(node);
}

void Directory::removeNode(FileSystemNode* node) {
    nodes.removeOne(node);
}

QList<FileSystemNode*> Directory::children() const {
    return nodes;
}
