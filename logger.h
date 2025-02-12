#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QDateTime>
#include <QMutex>

// Singleton Logger class for logging messages to a file.
class Logger {
public:
    // Enumeration for different log levels
    enum LogLevel {
        LOG_INFO,      // Info level log
        LOG_WARNING,   // Warning level log
        LOG_ERROR      // Error level log
    };

    // Returns the unique instance of the Logger
    static Logger& getInstance();

    // Logs a message with a specific log level and optional source
    void log(LogLevel level, const QString& message, const QString& source = "");

private:
    // Private constructor for singleton
    Logger();
    // Private destructor for singleton
    ~Logger();
    // Prevent copy constructor
    Logger(const Logger&) = delete;
    // Prevent copy assignment operator
    Logger& operator=(const Logger&) = delete;

    // Initializes the log file with a given file name
    void initLogFile(const QString& fileName);

    QFile logFile;  // The file where log messages are written
    QMutex mutex;   // Mutex to ensure thread-safety during logging
};

#endif // LOGGER_H
