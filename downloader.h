#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>
#include <vector>

class Downloader {
public:
    Downloader(); // Constructor initializes cURL and logging
    ~Downloader(); // Destructor cleans up cURL resources

    // Download data from all predefined URLs and return file paths of downloaded data
    std::vector<std::string> downloadAllData();

private:
    // Helper functions for file and directory management
    std::string getDataFolderPath() const; // Get path to data folder
    bool ensureDataFolderExists() const; // Ensure that the data folder exists
    bool downloadData(const std::string& url, const std::string& filePath) const; // Download data from URL
    bool saveDataToFile(const std::string& data, const std::string& filename) const; // Save data to file
    std::string getFileExtension(const std::string& url) const; // Extract file extension based on URL

    // List of URLs for universities to download data from
    const std::vector<std::string> urls = {
        "https://chromium-case-study.s3.us-east-1.amazonaws.com/candidate+feeds/Polytechnic-University-of-Bucharest.xml",
        "https://chromium-case-study.s3.us-east-1.amazonaws.com/candidate+feeds/University-of-S%C3%A3o-Paulo.xml",
        "https://chromium-case-study.s3.us-east-1.amazonaws.com/candidate+feeds/University-of-Florida.json",
        "https://chromium-case-study.s3.us-east-1.amazonaws.com/candidate+feeds/University-of-Havana.json"
    };
};

#endif // DOWNLOADER_H
