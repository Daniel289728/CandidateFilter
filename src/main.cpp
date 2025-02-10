#include <iostream>
#include <cstdlib>  // For std::setenv (or _putenv in Windows)
#include "downloader.h"
#include "parser.h"
#include "candidateFilter.h"

int main(void) {
    Downloader downloader;
    Parser parser;

    // Get the list of downloaded files
    std::vector<std::string> data = downloader.downloadAllData();

    // Process the downloaded files (pass by reference)
    std::vector<Candidate> candidates = parser.processDownloadedData(data);


    std::cout << "Candidates with GPA >= 3.9:\n";
    for (const auto& c : CandidateFilter::filterByGPA(candidates, 3.9)) {
        c.print();
    }

    std::cout << "\nCandidates with 'Python' skill:\n";
    for (const auto& c : CandidateFilter::filterBySkill(candidates, "Python")) {
        c.print();
    }

    std::cout << "\nCandidates from University Of Sao Paulo:\n";
    for (const auto& c : CandidateFilter::filterByUniversity(candidates, "University Of Sao Paulo")) {
        c.print();
    }

    return 0;
}