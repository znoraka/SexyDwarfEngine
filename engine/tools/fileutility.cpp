#include "fileutility.h"

QString const FileUtility::tempFileName = "./bnsposnp";


QString FileUtility::loadTempFile(QString resourcePath)
{
    QFile tempFile(tempFileName);
    tempFile.remove();
    QFile f(resourcePath);
    //        QString tempFilePath(tempFileName);
    f.copy(tempFileName);

    return tempFileName;
}
