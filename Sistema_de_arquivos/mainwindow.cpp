#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "file.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QVBoxLayout>
#include <QPushButton>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), rootDir(new Directory("Root"))
{
    ui->setupUi(this);
    setupUI();
}

MainWindow::~MainWindow() {
    delete ui;
    delete rootDir;
}

void MainWindow::setupUI() {
    // Configurar o QTreeView para exibir o sistema de arquivos
    fileTreeView = new QTreeView(this);
    fileModel = new QFileSystemModel(this);
    fileModel->setRootPath("");  // Use o caminho raiz real do sistema
    fileTreeView->setModel(fileModel);
    fileTreeView->setRootIndex(fileModel->index(fileModel->rootPath()));

    // Botões para criar, excluir e redimensionar arquivos
    QPushButton* createButton = new QPushButton("Criar Arquivo", this);
    QPushButton* deleteButton = new QPushButton("Excluir Arquivo", this);
    QPushButton* resizeButton = new QPushButton("Redimensionar Arquivo", this);

    // Conectar os botões aos slots correspondentes
    connect(createButton, &QPushButton::clicked, this, &MainWindow::createFile);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteFile);
    connect(resizeButton, &QPushButton::clicked, this, &MainWindow::resizeFile);

    // Layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(fileTreeView);
    layout->addWidget(createButton);
    layout->addWidget(deleteButton);
    layout->addWidget(resizeButton);

    QWidget* container = new QWidget;
    container->setLayout(layout);
    setCentralWidget(container);
}

void MainWindow::createFile() {
    bool ok;
    QString name = QInputDialog::getText(this, "Criar Arquivo", "Nome do arquivo:", QLineEdit::Normal, "", &ok);
    if (ok && !name.isEmpty()) {
        int size = QInputDialog::getInt(this, "Tamanho do Arquivo", "Tamanho inicial (em KB):", 100, 0, 10000, 1, &ok);
        if (ok) {
            File* newFile = new File(name, size);
            rootDir->addNode(newFile);
            // Atualizar a exibição do modelo, se necessário
            fileModel->setRootPath(fileModel->rootPath());
        }
    }
}

void MainWindow::deleteFile() {
    QModelIndex index = fileTreeView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erro", "Nenhum arquivo selecionado.");
        return;
    }

    QString fileName = fileModel->fileName(index);
    int result = QMessageBox::warning(this, "Excluir Arquivo", "Tem certeza que deseja excluir " + fileName + "?",
                                      QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        // Remover o arquivo da estrutura de diretório
        FileSystemNode* node = rootDir->children().at(index.row());  // Achar o arquivo baseado no índice
        rootDir->removeNode(node);
        fileModel->remove(index);
    }
}

void MainWindow::resizeFile() {
    QModelIndex index = fileTreeView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erro", "Nenhum arquivo selecionado.");
        return;
    }

    bool ok;
    int newSize = QInputDialog::getInt(this, "Redimensionar Arquivo", "Novo tamanho (em KB):", 100, 0, 10000, 1, &ok);
    if (ok) {
        // Encontrar o arquivo correspondente
        File* selectedFile = static_cast<File*>(rootDir->children().at(index.row()));
        selectedFile->setSize(newSize);

        // Mostrar barra de progresso como feedback visual
        QProgressDialog progress("Redimensionando arquivo...", "Cancelar", 0, newSize, this);
        progress.setWindowModality(Qt::WindowModal);
        for (int i = 0; i < newSize; ++i) {
            progress.setValue(i);
            QCoreApplication::processEvents();
            if (progress.wasCanceled()) {
                break;
            }
        }
        progress.setValue(newSize);
    }
}
