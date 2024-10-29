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
    // Verifica se o nó já está presente, caso não sejam permitidos duplicados
    if (!nodes.contains(node)) {
        nodes.push_back(node);
    }
}

void Directory::removeNode(FileSystemNode* node) {
    if (nodes.contains(node)) {
        nodes.removeOne(node);
        delete node;  // Libera a memória do nó
    }
}

QList<FileSystemNode*> Directory::children() const {
    return nodes;
}
