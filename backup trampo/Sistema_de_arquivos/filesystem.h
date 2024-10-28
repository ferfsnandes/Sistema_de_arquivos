#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <QMainWindow>

// Classe base para Arquivo e Diretório
class Node {
public:
    std::string name;
    bool isFile;

    Node(std::string n, bool file) : name(n), isFile(file) {}
    virtual ~Node() {}
    virtual void list(int indent = 0) const = 0;
    virtual Node* search(const std::string& n) = 0;
};

// Classe para Arquivo
class File : public Node {
public:
    int size;

    File(std::string n, int s) : Node(n, true), size(s) {}

    void resize(int newSize) {
        size = newSize;
    }

    void list(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "- " << name << " (Arquivo, " << size << " bytes)" << std::endl;
    }

    Node* search(const std::string& n) override {
        return (name == n) ? this : nullptr;
    }
};

// Classe para Diretório
class Directory : public Node {
public:
    std::vector<Node*> children;

    Directory(std::string n) : Node(n, false) {}

    ~Directory() {
        for (auto child : children)
            delete child;
    }

    void add(Node* node) {
        children.push_back(node);
    }

    void remove(const std::string& n) {
        auto it = std::remove_if(children.begin(), children.end(),
                                 [&n](Node* node) { return node->name == n; });
        if (it != children.end()) {
            delete *it;
            children.erase(it, children.end());
        }
    }

    void list(int indent = 0) const override {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << "+ " << name << " (Diretório)" << std::endl;
        for (const auto& child : children) {
            child->list(indent + 1);
        }
    }

    Node* search(const std::string& n) override {
        if (name == n) return this;
        for (auto child : children) {
            Node* result = child->search(n);
            if (result) return result;
        }
        return nullptr;
    }
};

// Funções utilitárias
void createFile(Directory* dir, const std::string& fileName, int size) {
    dir->add(new File(fileName, size));
}

void deleteFile(Directory* dir, const std::string& fileName) {
    dir->remove(fileName);
}

void resizeFile(Directory* dir, const std::string& fileName, int newSize) {
    Node* result = dir->search(fileName);
    if (result && result->isFile) {
        File* file = static_cast<File*>(result);
        file->resize(newSize);
    }
}

#endif // FILESYSTEM_H


