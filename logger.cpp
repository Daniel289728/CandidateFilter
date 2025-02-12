#include "Logger.h"
#include <QTextStream>

Logger::Logger() {
    QString logFileName = "candidate_feed_errors.log";
    logFile.setFileName(logFileName);
    logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
}

Logger::~Logger() {
    if (logFile.isOpen()) {
        logFile.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(LogLevel level, const QString& message, const QString& source) {
    QMutexLocker locker(&mutex);

    QString levelStr;
    switch (level) {
    case LogLevel::LOG_INFO: levelStr = "INFO"; break;
    case LogLevel::LOG_WARNING: levelStr = "WARNING"; break;
    case LogLevel::LOG_ERROR: levelStr = "ERROR"; break;
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString logEntry = QString("[%1] [%2] %3: %4\n")
                           .arg(timestamp)
                           .arg(levelStr)
                           .arg(source)
                           .arg(message);

    if (logFile.isOpen()) {
        QTextStream stream(&logFile);
        stream << logEntry;
        stream.flush();
    }
}
