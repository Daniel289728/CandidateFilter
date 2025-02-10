#include <iostream>
#include <cstdlib>  // For std::setenv (or _putenv in Windows)
#include "downloader.h"

int main(void) {

    Downloader downloader;

    if (downloader.downloadAllData()) {
        std::cout << "All candidate data downloaded successfully!" << std::endl;
    }
    else {
        std::cout << "Failed to download candidate data." << std::endl;
    }

    return 0;
}
