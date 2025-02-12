#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <nlohmann/json.hpp>  // For JSON parsing
#include <pugixml.hpp>        // For XML parsing
#include "candidate.h"

class Parser
{
public:
    // Constructor and Destructor
    Parser();
    ~Parser();
    
    // Main function to process the downloaded data from files
    std::vector<Candidate> processDownloadedData(const std::vector<std::string>& dataFiles);

private:
    // Helper function to extract the university name from the file path
    std::string extractUniversityName(const std::string& filePath);
    
    // Helper functions for parsing different file formats
    std::vector<Candidate> parseXML(const std::string& filePath);
    std::vector<Candidate> parseJSON(const std::string& filePath);
    void parseSkills(Candidate& candidate, const pugi::xml_node& candidateNode);
    void parseHobby(Candidate& candidate, const pugi::xml_node& candidateNode);
};

#endif // PARSER_H
