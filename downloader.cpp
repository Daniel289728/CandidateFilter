#include "downloader.h"
#include <curl/curl.h>
#include <fstream>
#include <sstream>
#include <filesystem>
Downloader::Downloader() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    Logger::getInstance().log(Logger::LOG_INFO, 
        QString("Downloader initialized with cURL"), 
        QString("Downloader"));
}

Downloader::~Downloader() {
    curl_global_cleanup();
    Logger::getInstance().log(Logger::LOG_INFO, 
        QString("Downloader cleanup completed"), 
        QString("Downloader"));
}

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    std::string* data = static_cast<std::string*>(userp);
    data->append(static_cast<char*>(contents), total_size);
    return total_size;
}

bool Downloader::downloadData(const std::string& url, const std::string& filePath) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        Logger::getInstance().log(Logger::LOG_ERROR, 
            QString("Failed to initialize cURL"), 
            QString("Downloader"));
        return false;
    }

    std::string response_data;
    FILE* file = fopen(filePath.c_str(), "wb");
    if (!file) {
        Logger::getInstance().log(Logger::LOG_ERROR,
            QString("Failed to open file for writing: %1").arg(QString::fromStdString(filePath)),
            QString("Downloader"));
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Disable SSL certificate verification
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        Logger::getInstance().log(Logger::LOG_ERROR,
            QString("Failed to download data from %1. Error: %2")
                .arg(QString::fromStdString(url))
                .arg(curl_easy_strerror(res)),
            QString("Downloader"));
        fclose(file);
        curl_easy_cleanup(curl);
        return false;
    }

    size_t written = fwrite(response_data.c_str(), 1, response_data.size(), file);
    if (written != response_data.size()) {
        Logger::getInstance().log(Logger::LOG_ERROR,
            QString("Error writing to file: %1").arg(QString::fromStdString(filePath)),
            QString("Downloader"));
        fclose(file);
        curl_easy_cleanup(curl);
        return false;
    }

    fclose(file);
    curl_easy_cleanup(curl);

    Logger::getInstance().log(Logger::LOG_INFO,
        QString("Successfully downloaded data from %1 to %2")
            .arg(QString::fromStdString(url))
            .arg(QString::fromStdString(filePath)),
        QString("Downloader"));

    return true;
}

// Ensure the data folder exists before downloading
bool Downloader::ensureDataFolderExists() {
    std::string dataFolderPath = getDataFolderPath(); // Get the dynamic path to data folder
    std::filesystem::path dataFolder(dataFolderPath);

    // Check if the directory exists, create it if it doesn't
    if (!std::filesystem::exists(dataFolder)) {
        Logger::getInstance().log(Logger::LOG_INFO,
            QString("Data folder does not exist. Creating directory: %1")
                .arg(QString::fromStdString(dataFolderPath)),
            QString("Downloader"));
        try {
            std::filesystem::create_directory(dataFolder);
        } catch (const std::filesystem::filesystem_error& e) {
            Logger::getInstance().log(Logger::LOG_ERROR,
                QString("Failed to create data directory: %1")
                    .arg(QString::fromStdString(dataFolderPath)),
                QString("Downloader"));
            return false;
        }
    } else {
        Logger::getInstance().log(Logger::LOG_INFO,
            QString("Data folder already exists: %1")
                .arg(QString::fromStdString(dataFolderPath)),
            QString("Downloader"));
    }

    return true;
}

// Get the path to the 'data' folder relative to the executable
std::string Downloader::getDataFolderPath() {
    std::filesystem::path exePath = std::filesystem::current_path(); // Get the current working directory (where the executable is)
    std::filesystem::path dataFolderPath = exePath.parent_path() / "data"; // Go up one level and find the "data" folder
    return dataFolderPath.string(); // Return the path as a string
}

// Download data from all URLs and save to files
std::vector<std::string> Downloader::downloadAllData() {
    std::vector<std::string> downloadedFiles; // Vector to hold downloaded file paths
    bool success = true;

    // Ensure the data folder exists before downloading
    if (!ensureDataFolderExists()) {
        Logger::getInstance().log(Logger::LOG_ERROR,
            QString("Failed to ensure data folder exists"),
            QString("Downloader"));
        return downloadedFiles;
    }

    // Get the data folder path dynamically
    std::string dataFolderPath = getDataFolderPath();
    Logger::getInstance().log(Logger::LOG_INFO,
        QString("Using data folder path: %1")
            .arg(QString::fromStdString(dataFolderPath)),
        QString("Downloader"));

    // Hardcoded university names based on known URLs
    std::vector<std::string> universityNames = {
        "Polytechnic-University-of-Bucharest",
        "University-of-Florida",
        "University-of-Havana",
        "University-of-São-Paulo"
    };

    for (size_t i = 0; i < urls.size(); ++i) {
        if (i >= universityNames.size()) {
            Logger::getInstance().log(Logger::LOG_ERROR,
                QString("More URLs than expected university names!"),
                QString("Downloader"));
            break;
        }

        std::string filename = dataFolderPath + "/" + universityNames[i] + getFileExtension(urls[i]);
        Logger::getInstance().log(Logger::LOG_INFO,
            QString("Attempting to download data to: %1")
                .arg(QString::fromStdString(filename)),
            QString("Downloader"));

        if (downloadData(urls[i], filename)) {
            downloadedFiles.push_back(filename);  // Add successful file path to vector
        }
        else {
            Logger::getInstance().log(Logger::LOG_ERROR,
                QString("Failed to download data from %1")
                    .arg(QString::fromStdString(urls[i])),
                QString("Downloader"));
            success = false;
        }
    }

    if (success) {
        Logger::getInstance().log(Logger::LOG_INFO,
            QString("Successfully downloaded %1 files")
                .arg(downloadedFiles.size()),
            QString("Downloader"));
    }

    return success ? downloadedFiles : std::vector<std::string>();
}



// Save data to a file (helper function)
bool Downloader::saveDataToFile(const std::string& data, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        Logger::getInstance().log(Logger::LOG_ERROR,
            QString("Failed to open file for writing: %1")
                .arg(QString::fromStdString(filename)),
            QString("Downloader"));
        return false;
    }
    outFile << data;
    outFile.close();

    Logger::getInstance().log(Logger::LOG_INFO,
        QString("Successfully saved data to file: %1")
            .arg(QString::fromStdString(filename)),
        QString("Downloader"));
    return true;
}

std::string Downloader::getFileExtension(const std::string& url) {
    if (url.find(".xml") != std::string::npos) {
        return ".xml";
    }
    else if (url.find(".json") != std::string::npos) {
        return ".json";
    }
    return ".data";  // Default extension
}
