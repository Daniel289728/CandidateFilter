#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>
#include <vector>

class Downloader {
public:
    Downloader();
    ~Downloader();

    // Add the declaration for the new method
    std::string getDataFolderPath();

    bool downloadData(const std::string& url, const std::string& filePath);
    bool downloadAllData();
    bool Downloader::ensureDataFolderExists();
private:
    bool saveDataToFile(const std::string& data, const std::string& filename);
    // List of URLs for the universities
    const std::vector<std::string> urls = {
        "https://chromium-case-study.s3.us-east-1.amazonaws.com/candidate+feeds/Polytechnic-University-of-Bucharest.xml",
        "https://chromium-case-study.s3.us-east-1.amazonaws.com/candidate+feeds/University-of-S%C3%A3o-Paulo.xml",
        "https://chromium-case-study.s3.us-east-1.amazonaws.com/candidate+feeds/University-of-Florida.json",
        "https://chromium-case-study.s3.us-east-1.amazonaws.com/candidate+feeds/University-of-Havana.json"
    };
};

#endif // DOWNLOADER_H
