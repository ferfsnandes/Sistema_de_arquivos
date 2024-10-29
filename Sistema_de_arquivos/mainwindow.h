#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QFileSystemModel>

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
    QTreeView* fileTreeView;
    QFileSystemModel* fileModel;

    void setupUI();  // Função para configurar a interface
};

#endif // MAINWINDOW_H
