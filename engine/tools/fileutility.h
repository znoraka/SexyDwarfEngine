#ifndef FILEUTILITY_H
#define FILEUTILITY_H

#include <QString>
#include <QFile>

class FileUtility
{
public:

    static const QString tempFileName;
    static QString loadTempFile(QString resourcePath);
};

#endif // FILEUTILITY_H
