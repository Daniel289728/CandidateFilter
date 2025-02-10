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
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "Failed to download data from " << url << " Error: " << curl_easy_strerror(res) << std::endl;
        fclose(file);
        curl_easy_cleanup(curl);
        return false;
    }

    fwrite(response_data.c_str(), 1, response_data.size(), file);
    fclose(file);
    curl_easy_cleanup(curl);

    return true;
}

// Get the path to the 'data' folder relative to the executable
std::string Downloader::getDataFolderPath() {
    std::filesystem::path exePath = std::filesystem::current_path(); // Get the current working directory (where the executable is)
    std::filesystem::path dataFolderPath = exePath.parent_path().parent_path() / "data"; // Go up two levels and find the "data" folder
    return dataFolderPath.string(); // Return the path as a string
}

// Download data from all URLs and save to files
bool Downloader::downloadAllData() {
    bool success = true;

    // Get the data folder path dynamically
    std::string dataFolderPath = getDataFolderPath();

    for (size_t i = 0; i < urls.size(); ++i) {
        // Construct the full path to the file in the 'data' folder
        std::string filename = dataFolderPath + "/university_" + std::to_string(i + 1) + ".data"; // Using dynamic path

        if (!downloadData(urls[i], filename)) {
            std::cerr << "Failed to download data from " << urls[i] << std::endl;
            success = false;
        }
    }
    return success;
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
