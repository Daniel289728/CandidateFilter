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
    bool saveDataToFile(const std::string& data, const std::string& filename);

private:
    std::vector<std::string> urls;
};

#endif // DOWNLOADER_H
