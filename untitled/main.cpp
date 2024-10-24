#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>
#include "filesystem.h"

class FileSystemUI : public QWidget {
    Q_OBJECT

public:
    FileSystemUI(QWidget* parent = nullptr) : QWidget(parent), root(new Directory("root")) {
        QVBoxLayout* layout = new QVBoxLayout(this);

        QLabel* titleLabel = new QLabel("Sistema de Arquivos Simples", this);
        layout->addWidget(titleLabel);

        fileListWidget = new QListWidget(this);
        layout->addWidget(fileListWidget);

        QPushButton* addButton = new QPushButton("Criar Arquivo", this);
        layout->addWidget(addButton);

        QPushButton* deleteButton = new QPushButton("Excluir Arquivo", this);
        layout->addWidget(deleteButton);

        QPushButton* resizeButton = new QPushButton("Redimensionar Arquivo", this);
        layout->addWidget(resizeButton);

        QPushButton* refreshButton = new QPushButton("Atualizar Lista", this);
        layout->addWidget(refreshButton);

        connect(addButton, &QPushButton::clicked, this, &FileSystemUI::onAddFileClicked);
        connect(deleteButton, &QPushButton::clicked, this, &FileSystemUI::onDeleteFileClicked);
        connect(resizeButton, &QPushButton::clicked, this, &FileSystemUI::onResizeFileClicked);
        connect(refreshButton, &QPushButton::clicked, this, &FileSystemUI::updateFileList);

        // Adiciona alguns arquivos de teste
        createFile(root, "file1.txt", 100);
        createFile(root, "file2.txt", 200);
        Directory* subDir = new Directory("subdir");
        root->add(subDir);
        createFile(subDir, "file3.txt", 300);

        updateFileList();
    }

private slots:
    void onAddFileClicked() {
        bool ok;
        QString fileName = QInputDialog::getText(this, "Criar Arquivo", "Nome do arquivo:", QLineEdit::Normal, "", &ok);
        if (ok && !fileName.isEmpty()) {
            int size = QInputDialog::getInt(this, "Criar Arquivo", "Tamanho do arquivo (bytes):", 0, 0, 10000, 1, &ok);
            if (ok) {
                createFile(root, fileName.toStdString(), size);
                updateFileList();
            }
        }
    }

    void onDeleteFileClicked() {
        QListWidgetItem* selectedItem = fileListWidget->currentItem();
        if (selectedItem) {
            QString fileName = selectedItem->text();
            deleteFile(root, fileName.toStdString());
            updateFileList();
        } else {
            QMessageBox::warning(this, "Erro", "Selecione um arquivo para excluir.");
        }
    }

    void onResizeFileClicked() {
        QListWidgetItem* selectedItem = fileListWidget->currentItem();
        if (selectedItem) {
            QString fileName = selectedItem->text();
            bool ok;
            int newSize = QInputDialog::getInt(this, "Redimensionar Arquivo", "Novo tamanho (bytes):", 0, 0, 10000, 1, &ok);
            if (ok) {
                resizeFile(root, fileName.toStdString(), newSize);
                updateFileList();
            }
        } else {
            QMessageBox::warning(this, "Erro", "Selecione um arquivo para redimensionar.");
        }
    }

    void updateFileList() {
        fileListWidget->clear();
        for (auto& child : root->children) {
            if (child->isFile) {
                fileListWidget->addItem(QString::fromStdString(child->name));
            }
        }
    }

private:
    Directory* root;
    QListWidget* fileListWidget;
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    FileSystemUI window;
    window.setWindowTitle("Sistema de Arquivos");
    window.resize(400, 300);
    window.show();

    return app.exec();
}

#include "main.moc"
