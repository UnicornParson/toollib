/*
  Copyright (c) 2010 Boris Moiseev (cyberbobs at gmail dot com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 2.1
  as published by the Free Software Foundation and appearing in the file
  LICENSE.LGPL included in the packaging of this file.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
*/
#ifndef FILEAPPENDER_H
#define FILEAPPENDER_H

// Logger
#include "CuteLogger_global.h"
#include "AbstractStringAppender.h"

// Qt
#include <QFile>
#include <QTextStream>


class  FileAppender : public AbstractStringAppender
{
public:
    FileAppender(const QString& fileName = QString());
    ~FileAppender();

    QString fileName() const;
    void setFileName(const QString&);
    void setLogFileSizeLimitMB(int sizeLimit);
protected:
    virtual void append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                        const char* function, const QString& category, const QString& message);
    bool openFile();
    void closeFile();
    void checkFileSizeLimit();
private:
    QFile m_logFile;
    QTextStream m_logStream;
    mutable QMutex m_logFileMutex;
    QString m_logFileName;
    int m_sizeLimit;
    int m_currentFileIndex;
};

#endif // FILEAPPENDER_H
