#include "parser.h"
#include <iostream>
#include <fstream>
#include <algorithm>

Parser::Parser() {
}

Parser::~Parser() {
}

std::vector<Candidate> Parser::parseXML(const std::string& filePath) {
    std::vector<Candidate> candidates;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());

    if (!result) {
        std::cerr << "Failed to load XML file: " << filePath << " Error: " << result.description() << std::endl;
        return candidates;
    }

    // Check if <root> node exists
    pugi::xml_node rootNode = doc.child("root");
    if (!rootNode) {
        std::cerr << "Error: Missing <root> node in XML file: " << filePath << std::endl;
        return candidates;
    }

    for (pugi::xml_node candidateNode = rootNode.child("candidate"); candidateNode; 
         candidateNode = candidateNode.next_sibling("candidate")) {
        Candidate candidate;
        candidate.name = candidateNode.child("name").text().as_string();
        candidate.GPA = candidateNode.child("GPA").text().as_double();

        pugi::xml_node skillsNode = candidateNode.child("skills");
        for (pugi::xml_node skill = skillsNode.child("skill"); skill; skill = skill.next_sibling("skill")) {
            candidate.skills.push_back(skill.text().as_string());
        }

        candidate.hobby = candidateNode.child("hobby").text().as_string();

        candidates.push_back(candidate);
    }

    return candidates;
}

std::vector<Candidate> Parser::parseJSON(const std::string& filePath) {
    std::vector<Candidate> candidates;
    std::ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open JSON file: " << filePath << std::endl;
        return candidates;
    }

    nlohmann::json jsonData;
    inputFile >> jsonData;

    for (const auto& candidate : jsonData) {
        Candidate c;
        if (candidate.contains("name")) c.name = candidate["name"];
        if (candidate.contains("GPA")) c.GPA = candidate["GPA"];
        if (candidate.contains("skills")) c.skills = candidate["skills"].get<std::vector<std::string>>();
        if (candidate.contains("hobby")) c.hobby = candidate["hobby"];

        candidates.push_back(c);
    }

    return candidates;
}

std::vector<Candidate> Parser::processDownloadedData(const std::vector<std::string>& dataFiles) {
    std::vector<Candidate> allCandidates;

    for (const auto& filePath : dataFiles) {
        std::cout << "Processing: " << filePath << std::endl;

        // Extract university name from the file
        std::string universityName = extractUniversityName(filePath);
        std::cout << "University: " << universityName << std::endl;

        std::vector<Candidate> candidates;

        if (filePath.substr(filePath.find_last_of(".") + 1) == "xml") {
            candidates = parseXML(filePath);
        } else if (filePath.substr(filePath.find_last_of(".") + 1) == "json") {
            candidates = parseJSON(filePath);
        }

        // Assign university name to each candidate (if necessary)
        for (auto& candidate : candidates) {
            candidate.university = universityName;  // Assuming 'university' is a field in Candidate class
        }

        allCandidates.insert(allCandidates.end(), candidates.begin(), candidates.end());
    }

    return allCandidates;
}

// Function to extract the university name from the file path
#include <algorithm>  // For std::replace
#include <filesystem> // For filesystem::path

std::string Parser::extractUniversityName(const std::string& filePath) {
    std::filesystem::path path(filePath);

    // Get the filename without extension
    std::string filename = path.stem().string();

    // Replace hyphens or underscores with spaces
    std::replace(filename.begin(), filename.end(), '-', ' ');
    std::replace(filename.begin(), filename.end(), '_', ' ');

    // Replace 'ã' with 'a'
    std::replace(filename.begin(), filename.end(), 'ã', 'a');

    // Capitalize the first letter of each word for better readability (optional)
    bool capitalizeNext = true;
    for (char& ch : filename) {
        if (capitalizeNext && std::isalpha(ch)) {
            ch = std::toupper(ch);
            capitalizeNext = false;
        }
        else if (std::isspace(ch)) {
            capitalizeNext = true;
        }
    }

    return filename;
}

