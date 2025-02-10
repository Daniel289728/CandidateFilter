#include <iostream>
#include <cstdlib>  // For std::setenv (or _putenv in Windows)
#include "downloader.h"
#include "parser.h"

int main(void) {
    Downloader downloader;
    Parser parser;

    // Get the list of downloaded files
    std::vector<std::string> data = downloader.downloadAllData();

    // Process the downloaded files (pass by reference)
    parser.processDownloadedData(data);

    return 0;
}