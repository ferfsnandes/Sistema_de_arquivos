#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "file.h"
#include "filesystemmodel.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QToolBar>
#include <QAction>
#include <QSplitter>
#include <QVBoxLayout>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), rootDir(new Directory("Root"))
{
    ui->setupUi(this);
    setupUI();
    setupMenus();
}

MainWindow::~MainWindow() {
    delete ui;
    delete rootDir;
}





void MainWindow::setupUI() {
    QSplitter *splitter = new QSplitter(this);

    directoryTreeView = new QTreeView(this);
    fileModel = new FileSystemModel(rootDir, this);
    directoryTreeView->setModel(fileModel);

    fileListView = new QListView(splitter);
    fileListView->setModel(fileModel);

    splitter->addWidget(directoryTreeView);
    splitter->addWidget(fileListView);

    setCentralWidget(splitter);
}


void MainWindow::setupMenus() {
    // Barra de ferramentas com ações de criação, exclusão e redimensionamento
    QToolBar *toolbar = addToolBar("File Operations");

    QAction *createFileAction = new QAction("Criar Arquivo", this);
    connect(createFileAction, &QAction::triggered, this, &MainWindow::createFile);
    toolbar->addAction(createFileAction);

    QAction *deleteFileAction = new QAction("Excluir Arquivo", this);
    connect(deleteFileAction, &QAction::triggered, this, &MainWindow::deleteFile);
    toolbar->addAction(deleteFileAction);

    QAction *resizeFileAction = new QAction("Redimensionar Arquivo", this);
    connect(resizeFileAction, &QAction::triggered, this, &MainWindow::resizeFile);
    toolbar->addAction(resizeFileAction);
}

void MainWindow::createFile() {
    bool ok;
    QString name = QInputDialog::getText(this, "Criar Arquivo", "Nome do arquivo:", QLineEdit::Normal, "", &ok);
    if (ok && !name.isEmpty()) {
        int size = QInputDialog::getInt(this, "Tamanho do Arquivo", "Tamanho inicial (em KB):", 100, 0, 10000, 1, &ok);
        if (ok) {
            fileModel->addFile(name, size);  // Chamada ao modelo personalizado
        }
    }
}

void MainWindow::deleteFile() {
    QModelIndex index = directoryTreeView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erro", "Nenhum arquivo selecionado.");
        return;
    }

    fileModel->deleteFile(index);
}
void MainWindow::resizeFile() {
    QModelIndex index = directoryTreeView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erro", "Nenhum arquivo selecionado.");
        return;
    }

    int rowIndex = index.row();
    if (rowIndex < 0 || rowIndex >= rootDir->children().size()) {
        QMessageBox::warning(this, "Erro", "Índice fora do intervalo.");
        return;
    }

    bool ok;
    int newSize = QInputDialog::getInt(this, "Redimensionar Arquivo", "Novo tamanho (em KB):", 100, 0, 10000, 1, &ok);
    if (ok) {
        File* selectedFile = dynamic_cast<File*>(rootDir->children().at(rowIndex));
        if (selectedFile) {
            selectedFile->setSize(newSize);

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
        } else {
            QMessageBox::warning(this, "Erro", "O item selecionado não é um arquivo.");
        }
    }
}
