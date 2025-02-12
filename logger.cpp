#include "Logger.h"
#include <QTextStream>

Logger::Logger() {
    // Initialize log file with default name
    initLogFile("candidate_feed_errors.log");
}

Logger::~Logger() {
    // Close the log file if it's open
    if (logFile.isOpen()) {
        logFile.close();
    }
}

// Initializes the log file with the provided file name
void Logger::initLogFile(const QString& fileName) {
    logFile.setFileName(fileName);
    // Open the log file for appending text
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        // Log a warning if the log file cannot be opened
        qWarning() << "Failed to open log file for writing.";
    }
}

Logger& Logger::getInstance() {
    // Use static instance to ensure only one logger instance is created
    static Logger instance;
    return instance;
}

void Logger::log(LogLevel level, const QString& message, const QString& source) {
    // Lock the mutex to ensure thread-safety during logging
    QMutexLocker locker(&mutex);

    // Determine the log level as a string
    QString levelStr;
    switch (level) {
    case LogLevel::LOG_INFO:
        levelStr = "INFO";
        break;
    case LogLevel::LOG_WARNING:
        levelStr = "WARNING";
        break;
    case LogLevel::LOG_ERROR:
        levelStr = "ERROR";
        break;
    default:
        levelStr = "UNKNOWN";
    }

    // Get the current timestamp for the log entry
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    // Format the log entry
    QString logEntry = QString("[%1] [%2] %3: %4\n")
                           .arg(timestamp)
                           .arg(levelStr)
                           .arg(source)
                           .arg(message);

    // Write the log entry to the file if it's open
    if (logFile.isOpen()) {
        QTextStream stream(&logFile);
        stream << logEntry;
        stream.flush();  // Ensure immediate write to disk
    } 
	else {
        // Log a warning if the log file is not open
        qWarning() << "Log file is not open. Log entry could not be written.";
    }
}
