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

    FileSystemNode* binarySearch(const QString& fileName);  // Função de busca binária

private:
    QString dirName;
    QList<FileSystemNode*> nodes;

    void sortNodes();  // Função para ordenar os arquivos e diretórios alfabeticamente
};

#endif // DIRECTORY_H
