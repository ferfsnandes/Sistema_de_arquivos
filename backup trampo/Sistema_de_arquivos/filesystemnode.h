#ifndef FILESYSTEMNODE_H
#define FILESYSTEMNODE_H

#include <QString>

class FileSystemNode {
public:
    virtual ~FileSystemNode() {}
    virtual QString name() const = 0;
    virtual bool isDirectory() const = 0;
};

#endif // FILESYSTEMNODE_H
