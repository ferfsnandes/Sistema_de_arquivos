#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QListView>
#include <QSplitter>
#include "directory.h"
#include "filesystemmodel.h"  // Certifique-se de incluir o modelo personalizado

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void createFile();
    void deleteFile();
    void resizeFile();

private:
    Ui::MainWindow *ui;
    Directory* rootDir;
    QTreeView* directoryTreeView;
    QListView* fileListView;
    FileSystemModel* fileModel;

    void setupUI();
    void setupMenus();
};

#endif // MAINWINDOW_H
