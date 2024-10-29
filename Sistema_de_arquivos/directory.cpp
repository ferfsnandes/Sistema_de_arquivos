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
         sortNodes();  // Ordena os nós após a adição de um novo arquivo
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

// Função para ordenar os nós alfabeticamente pelo nome
void Directory::sortNodes() {
    std::sort(nodes.begin(), nodes.end(), [](FileSystemNode* a, FileSystemNode* b) {
        return a->name().toLower() < b->name().toLower();
    });
}

// Implementação da busca binária para encontrar um arquivo pelo nome
FileSystemNode* Directory::binarySearch(const QString& fileName) {
    int left = 0;
    int right = nodes.size() - 1;

    while (left <= right) {
        int mid = (left + right) / 2;
        QString midName = nodes[mid]->name().toLower();

        if (midName == fileName.toLower()) {
            return nodes[mid];  // Retorna o nó encontrado
        } else if (midName < fileName.toLower()) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return nullptr;  // Retorna nullptr se o arquivo não for encontrado
}
