#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QFileSystemModel>
#include <QLineEdit>
#include <QPushButton>
#include "file.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

/*public slots:
    void onFileSizeChanged(const QString& filePath, int newSize);*/

private slots:
    void createFile();
    void deleteFile();
    void resizeFile();
    void onFileSizeChanged(const QString& fileName, int newSize);
    void searchFile();
private:
    File* file;
    Ui::MainWindow *ui;
    QTreeView* fileTreeView;
    QFileSystemModel* fileModel;
    QLineEdit* searchField;
    QPushButton* searchButton;

    void setupUI();  // Função para configurar a interface
};

#endif // MAINWINDOW_H
