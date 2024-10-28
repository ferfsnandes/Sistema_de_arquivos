#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QAbstractItemModel>
#include "directory.h"

class FileSystemModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit FileSystemModel(Directory* root, QObject* parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addFile(const QString& name, int size);
    void deleteFile(const QModelIndex& index);

private:
    Directory* rootDir;
};

#endif // FILESYSTEMMODEL_H
