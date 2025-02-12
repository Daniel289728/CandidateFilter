#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QDateTime>
#include <QMutex>
#include <memory>

class Logger {
public:
    enum LogLevel {
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR
    };

    static Logger& getInstance();
    void log(LogLevel level, const QString& message, const QString& source = "");

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    QFile logFile;
    QMutex mutex;
};
#endif // LOGGER_H
