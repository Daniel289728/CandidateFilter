#include "parser.h"
#include <iostream>
#include <fstream>

Parser::Parser() {
}

Parser::~Parser() {
}

void Parser::parseXML(const std::string& filePath) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());

    if (!result) {
        std::cerr << "Failed to load XML file: " << filePath 
                  << " Error: " << result.description() 
                  << " At: " << result.offset << std::endl;
        return;
    }

    // Check if <root> node exists
    pugi::xml_node rootNode = doc.child("root");
    if (!rootNode) {
        std::cerr << "Error: Missing <root> node in XML file: " << filePath << std::endl;
        return;
    }

    // Print the number of <candidate> nodes found
    int candidateCount = 0;

    // Loop through all <candidate> nodes under <root>
    for (pugi::xml_node candidateNode = rootNode.child("candidate"); candidateNode; candidateNode = candidateNode.next_sibling("candidate")) {
        candidateCount++;

        std::string name = candidateNode.child("name").text().as_string();
        double GPA = candidateNode.child("GPA").text().as_double();

        // Parse multiple <skills> tags
        std::vector<std::string> skills;
        for (pugi::xml_node skill : candidateNode.children("skills")) {
            skills.push_back(skill.text().as_string());
        }

        std::string hobby = candidateNode.child("hobby").text().as_string();

        // Output the parsed values
        std::cout << "Name: " << name << std::endl;
        std::cout << "GPA: " << GPA << std::endl;
        std::cout << "Skills: ";
        for (const auto& skill : skills) {
            std::cout << skill << " ";
        }
        std::cout << std::endl;
        std::cout << "Hobby: " << hobby << std::endl;
    }

    if (candidateCount == 0) {
        std::cerr << "No <candidate> nodes found in the XML file: " << filePath << std::endl;
    }
}


void Parser::parseJSON(const std::string& filePath) {
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open JSON file: " << filePath << std::endl;
        return;
    }

    nlohmann::json jsonData;
    inputFile >> jsonData;

    // Loop through each candidate in the JSON array
    for (const auto& candidate : jsonData) {
        std::string name = candidate.contains("name") ? candidate["name"] : "N/A"; // Default if missing
        double GPA = candidate.contains("GPA") ? candidate["GPA"] : -1.0; // Default GPA if missing
        std::vector<std::string> skills = candidate.contains("skills") ? candidate["skills"].get<std::vector<std::string>>() : std::vector<std::string>{};
        std::string hobby = candidate.contains("hobby") ? candidate["hobby"] : "N/A"; // Default if missing

        if (name == "N/A" || GPA == -1.0 || skills.empty()) {
            std::cerr << "Warning: Missing critical information for candidate. Skipping..." << std::endl;
            continue;  // Skip this candidate
        }

        // Output the parsed values
        std::cout << "Name: " << name << std::endl;
        std::cout << "GPA: " << GPA << std::endl;
        std::cout << "Skills: ";
        for (const auto& skill : skills) {
            std::cout << skill << " ";
        }
        std::cout << std::endl;
        std::cout << "Hobby: " << hobby << std::endl;
    }
}


void Parser::processDownloadedData(const std::vector<std::string>& dataFiles){
    for (const auto& filePath : dataFiles) {
        std::cout << "Processing: " << filePath << std::endl;
        if (filePath.substr(filePath.find_last_of(".") + 1) == "xml") {
            parseXML(filePath);
        }
        else if (filePath.substr(filePath.find_last_of(".") + 1) == "json") {
            parseJSON(filePath);
        }
    }
}