#include "downloader.h"
#include <curl/curl.h>
#include <fstream>
#include <filesystem>
#include "logger.h"

Downloader::Downloader() {
    curl_global_init(CURL_GLOBAL_DEFAULT); // Initialize global cURL environment
    Logger::getInstance().log(Logger::LOG_INFO, 
        QString("Downloader initialized with cURL"), 
        QString("Downloader"));
}

Downloader::~Downloader() {
    curl_global_cleanup(); // Clean up global cURL environment
    Logger::getInstance().log(Logger::LOG_INFO, 
        QString("Downloader cleanup completed"), 
        QString("Downloader"));
}

// cURL callback to write the downloaded data into a string
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    std::string* data = static_cast<std::string*>(userp);
    data->append(static_cast<char*>(contents), total_size); // Append data to the string
    return total_size;
}

// Download data from URL and save it to the given file path
bool Downloader::downloadData(const std::string& url, const std::string& filePath) const {
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
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback); // Set the write callback
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data); // Set the data string
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects

    // Disable SSL certificate verification (for simplicity, though it should be handled securely)
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

    fclose(file); // Close the file
    curl_easy_cleanup(curl); // Clean up cURL resources

    Logger::getInstance().log(Logger::LOG_INFO,
        QString("Successfully downloaded data from %1 to %2")
            .arg(QString::fromStdString(url))
            .arg(QString::fromStdString(filePath)),
        QString("Downloader"));

    return true;
}

// Ensure the data folder exists before downloading
bool Downloader::ensureDataFolderExists() const {
    std::string dataFolderPath = getDataFolderPath(); // Get the dynamic path to the data folder
    std::filesystem::path dataFolder(dataFolderPath);

    // If the folder doesn't exist, create it
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

// Get the path to the data folder relative to the executable
std::string Downloader::getDataFolderPath() const {
    std::filesystem::path exePath = std::filesystem::current_path(); // Get the current working directory
    std::filesystem::path dataFolderPath = exePath.parent_path() / "data"; // Set relative path to "data" folder
    return dataFolderPath.string();
}

// Download data from all predefined URLs and save to files
std::vector<std::string> Downloader::downloadAllData() {
    std::vector<std::string> downloadedFiles;
    if (!ensureDataFolderExists()) { // Ensure the data folder exists
        return downloadedFiles;
    }

    std::string dataFolderPath = getDataFolderPath(); // Get the path to the data folder
    Logger::getInstance().log(Logger::LOG_INFO,
        QString("Using data folder path: %1")
            .arg(QString::fromStdString(dataFolderPath)),
        QString("Downloader"));

    std::vector<std::string> universityNames = {
        "Polytechnic-University-of-Bucharest",
        "University-of-Florida",
        "University-of-Havana",
        "University-of-São-Paulo"
    };

    // Iterate over URLs to download data
    for (size_t i = 0; i < urls.size(); ++i) {
        if (i >= universityNames.size()) {
            Logger::getInstance().log(Logger::LOG_ERROR,
                QString("More URLs than expected university names!"),
                QString("Downloader"));
            break;
        }

        std::string filename = dataFolderPath + "/" + universityNames[i] + getFileExtension(urls[i]);
        if (downloadData(urls[i], filename)) {
            downloadedFiles.push_back(filename);
        }
        else {
            Logger::getInstance().log(Logger::LOG_ERROR,
                QString("Failed to download data from %1")
                    .arg(QString::fromStdString(urls[i])),
                QString("Downloader"));
        }
    }

    return downloadedFiles;
}

// Save data to a file
bool Downloader::saveDataToFile(const std::string& data, const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        Logger::getInstance().log(Logger::LOG_ERROR,
            QString("Failed to open file for writing: %1")
                .arg(QString::fromStdString(filename)),
            QString("Downloader"));
        return false;
    }
    outFile << data; // Write data to file
    outFile.close(); // Close file

    Logger::getInstance().log(Logger::LOG_INFO,
        QString("Successfully saved data to file: %1")
            .arg(QString::fromStdString(filename)),
        QString("Downloader"));
    return true;
}

// Get the file extension based on URL
std::string Downloader::getFileExtension(const std::string& url) const {
    if (url.find(".xml") != std::string::npos) {
        return ".xml";
    }
    else if (url.find(".json") != std::string::npos) {
        return ".json";
    }
    return ".data"; // Default extension if not found
}
