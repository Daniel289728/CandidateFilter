#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <nlohmann/json.hpp>  // For JSON parsing
#include <pugixml.hpp>        // For XML parsing
#include "candidate.h"

class Parser
{
public:
    Parser();
    ~Parser();
    std::vector<Candidate> processDownloadedData(const std::vector<std::string>& dataFiles);
private:
    std::string extractUniversityName(const std::string& filePath);
    std::vector<Candidate> parseXML(const std::string& filePath);
    std::vector<Candidate> parseJSON(const std::string& filePath);
};


#endif // PARSER_H
