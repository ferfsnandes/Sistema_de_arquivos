#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "file.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QProgressDialog>
#include <QVBoxLayout>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QLineEdit>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), file(new File("examplo.txt", 1024))
{
    ui->setupUi(this);
    setupUI();

    bool connected = connect(file, &File::sizeChanged, this, &MainWindow::onFileSizeChanged);
    qDebug() << "Signal-slot connection status:" << connected;
}

MainWindow::~MainWindow() {
    delete ui;
    delete file;
}

void MainWindow::setupUI() {
    // Configurar o QTreeView para exibir o sistema de arquivos
    fileTreeView = new QTreeView(this);
    fileModel = new QFileSystemModel(this);
    fileModel->setRootPath(QDir::rootPath());  // Use o caminho raiz do sistema operacional
    fileTreeView->setModel(fileModel);
    fileTreeView->setRootIndex(fileModel->index(QDir::rootPath()));

    // Botões para criar, excluir e redimensionar arquivos
    QPushButton* createButton = new QPushButton("Criar Arquivo", this);
    QPushButton* deleteButton = new QPushButton("Excluir Arquivo", this);
    QPushButton* resizeButton = new QPushButton("Redimensionar Arquivo", this);

    // Conectar os botões aos slots correspondentes
    connect(createButton, &QPushButton::clicked, this, &MainWindow::createFile);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteFile);
    connect(resizeButton, &QPushButton::clicked, this, &MainWindow::resizeFile);

    searchField = new QLineEdit(this);
    searchField->setPlaceholderText("Digite o nome do arquivo para buscar");

    searchButton = new QPushButton("Buscar", this);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchFile);

    // Layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(fileTreeView);
    layout->addWidget(createButton);
    layout->addWidget(deleteButton);
    layout->addWidget(resizeButton);
    layout->addWidget(searchField);
    layout->addWidget(searchButton);

    QWidget* container = new QWidget;
    container->setLayout(layout);
    setCentralWidget(container);
}

void MainWindow::createFile() {
    QModelIndex index = fileTreeView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erro", "Nenhum diretório selecionado.");
        return;
    }

    QString dirPath = fileModel->filePath(index);
    if (!QFileInfo(dirPath).isDir()) {
        QMessageBox::warning(this, "Erro", "A seleção atual não é um diretório.");
        return;
    }

    bool ok;
    QString name = QInputDialog::getText(this, "Criar Arquivo", "Nome do arquivo:", QLineEdit::Normal, "", &ok);
    if (ok && !name.isEmpty()) {
        QString filePath = QDir(dirPath).filePath(name);
        QFile file(filePath);

        if (file.exists()) {
            QMessageBox::warning(this, "Erro", "O arquivo já existe.");
            return;
        }

        if (file.open(QIODevice::WriteOnly)) {
            qDebug() << "Arquivo criado:" << filePath;
            file.close();
        } else {
            QMessageBox::warning(this, "Erro", "Falha ao criar o arquivo.");
        }

        fileModel->setRootPath(fileModel->rootPath());
    }
}

void MainWindow::deleteFile() {
    QModelIndex index = fileTreeView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erro", "Nenhum arquivo ou diretório selecionado.");
        return;
    }

    QString filePath = fileModel->filePath(index);
    QFileInfo fileInfo(filePath);

    int result = QMessageBox::warning(this, "Excluir Arquivo", "Tem certeza que deseja excluir " + fileInfo.fileName() + "?",
                                      QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        if (fileInfo.isFile()) {
            QFile file(filePath);
            if (!file.remove()) {
                QMessageBox::warning(this, "Erro", "Falha ao excluir o arquivo.");
            }
        } else if (fileInfo.isDir()) {
            QDir dir(filePath);
            if (!dir.removeRecursively()) {
                QMessageBox::warning(this, "Erro", "Falha ao excluir o diretório.");
            }
        }

        fileModel->setRootPath(fileModel->rootPath());
    }
}

void MainWindow::resizeFile() {
    QModelIndex index = fileTreeView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erro", "Nenhum arquivo selecionado.");
        return;
    }

    QString filePath = fileModel->filePath(index);
    QFileInfo fileInfo(filePath);

    if (!fileInfo.isFile()) {
        QMessageBox::warning(this, "Erro", "A seleção atual não é um arquivo.");
        return;
    }

    bool ok;
    int newSize = QInputDialog::getInt(this, "Redimensionar Arquivo", "Novo tamanho (em bytes):", fileInfo.size(), 0, 100000000, 1, &ok);
    if (ok) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(newSize);  // Resize the file directly
            fileModel->setRootPath(fileModel->rootPath());  // Refresh model to reflect new size in GUI
            qDebug() << "Arquivo redimensionado para:" << newSize << "bytes";
            file.close();
        } else {
            QMessageBox::warning(this, "Erro", "Falha ao redimensionar o arquivo.");
        }

        fileModel->setRootPath(fileModel->rootPath());
    }
}

void MainWindow::onFileSizeChanged(const QString& fileName, int newSize) {
    qDebug() << "File size changed for:" << fileName << "to" << newSize << "bytes";


    QModelIndex fileIndex = fileModel->index(QDir::rootPath() + "/" + fileName);

    if (fileIndex.isValid()) {
        emit fileModel->dataChanged(fileIndex, fileIndex);
    }
}


void MainWindow::searchFile() {
    QString partialFileName = searchField->text().trimmed();
    if (partialFileName.isEmpty()) {
        QMessageBox::warning(this, "Erro", "Digite parte do nome do arquivo para buscar.");
        return;
    }

    QModelIndex rootIndex = fileModel->index(QDir::rootPath());
    QModelIndex foundIndex;

    std::function<bool(const QModelIndex&)> searchRecursively = [&](const QModelIndex& parentIndex) -> bool {
        for (int i = 0; i < fileModel->rowCount(parentIndex); ++i) {
            QModelIndex childIndex = fileModel->index(i, 0, parentIndex);
            QString fileName = fileModel->fileName(childIndex);

            if (fileName.contains(partialFileName, Qt::CaseInsensitive)) {
                foundIndex = childIndex;
                return true;
            }

            if (fileModel->isDir(childIndex) && searchRecursively(childIndex)) {
                return true;
            }
        }
        return false;
    };

    if (searchRecursively(rootIndex)) {
        fileTreeView->setCurrentIndex(foundIndex);
        fileTreeView->scrollTo(foundIndex);
        QMessageBox::information(this, "Arquivo Encontrado", "Arquivo encontrado: " + fileModel->filePath(foundIndex));
    } else {
        QMessageBox::information(this, "Busca", "Arquivo não encontrado.");
    }
}
