#ifndef FILE_H
#define FILE_H

#include "filesystemnode.h"
#include <QObject>


class File : public QObject, public FileSystemNode {
    Q_OBJECT

public:
    File(const QString& name, int size);
    QString name() const override;
    bool isDirectory() const override;
    int size() const;
    void setSize(int newSize);

signals:
    void sizeChanged(const QString& fileName, int newSize);

private:
    QString fileName;
    int fileSize;
};

#endif // FILE_H
