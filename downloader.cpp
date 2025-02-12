#include "downloader.h"
#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>  // For filesystem handling

// Constructor
Downloader::Downloader() {
    // Initialize cURL library
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

// Destructor
Downloader::~Downloader() {
    // Clean up cURL
    curl_global_cleanup();
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
        std::cerr << "Failed to initialize cURL." << std::endl;
        return false;
    }

    std::string response_data;
    FILE* file = fopen(filePath.c_str(), "wb");
    if (!file) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        perror("Error");
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
        std::cerr << "Failed to download data from " << url << " Error: " << curl_easy_strerror(res) << std::endl;
        fclose(file);
        curl_easy_cleanup(curl);
        return false;
    }

    // Write the received data to the file
    size_t written = fwrite(response_data.c_str(), 1, response_data.size(), file);
    if (written != response_data.size()) {
        std::cerr << "Error writing to file: " << filePath << std::endl;
        fclose(file);
        curl_easy_cleanup(curl);
        return false;
    }

    fclose(file);
    curl_easy_cleanup(curl);

    return true;
}

// Ensure the data folder exists before downloading
bool Downloader::ensureDataFolderExists() {
    std::string dataFolderPath = getDataFolderPath(); // Get the dynamic path to data folder
    std::filesystem::path dataFolder(dataFolderPath);

    // Check if the directory exists, create it if it doesn't
    if (!std::filesystem::exists(dataFolder)) {
        std::cout << "Data folder does not exist. Creating directory: " << dataFolderPath << std::endl;
        std::filesystem::create_directory(dataFolder);
    }
    else {
        std::cout << "Data folder already exists: " << dataFolderPath << std::endl;
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
        std::cerr << "Failed to ensure data folder exists." << std::endl;
        return downloadedFiles; // Return empty vector on failure
    }

    // Get the data folder path dynamically
    std::string dataFolderPath = getDataFolderPath();
    std::cout << "Data folder path: " << dataFolderPath << std::endl;

    // Hardcoded university names based on known URLs
    std::vector<std::string> universityNames = {
        "Polytechnic-University-of-Bucharest",
        "University-of-Florida",
        "University-of-Havana",
        "University-of-São-Paulo"
    };

    for (size_t i = 0; i < urls.size(); ++i) {
        if (i >= universityNames.size()) {
            std::cerr << "Error: More URLs than expected university names!" << std::endl;
            break;
        }

        std::string filename = dataFolderPath + "/" + universityNames[i] + getFileExtension(urls[i]);
        std::cout << "Downloading data to: " << filename << std::endl;

        if (downloadData(urls[i], filename)) {
            downloadedFiles.push_back(filename);  // Add successful file path to vector
        }
        else {
            std::cerr << "Failed to download data from " << urls[i] << std::endl;
            success = false;
        }
    }

    return success ? downloadedFiles : std::vector<std::string>();
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



// Save data to a file (helper function)
bool Downloader::saveDataToFile(const std::string& data, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file " << filename << std::endl;
        return false;
    }
    outFile << data;
    outFile.close();
    return true;
}
