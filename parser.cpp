#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include "parser.h"
#include "pugixml.hpp"

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
        candidate.setName(candidateNode.child("name").text().as_string());
        candidate.setGPA(candidateNode.child("GPA").text().as_double());

        pugi::xml_node skillsNode = candidateNode.child("skills");
        for (pugi::xml_node skill = skillsNode.child("skill"); skill; skill = skill.next_sibling("skill")) {
            candidate.addSkill(skill.text().as_string());
        }

        candidate.setHobby(candidateNode.child("hobby").text().as_string());

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
        if (candidate.contains("name")) c.setName(candidate["name"]);
        if (candidate.contains("GPA")) c.setGPA(candidate["GPA"]);
        if (candidate.contains("skills")) c.setSkills(candidate["skills"].get<std::vector<std::string>>());
        if (candidate.contains("hobby")) c.setHobby(candidate["hobby"]);

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
            candidate.setUniversity(universityName);
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

    // Convert filename to wstring (wide string) for better Unicode handling
    std::wstring filename = path.stem().wstring();

    // Replace hyphens or underscores with spaces
    std::replace(filename.begin(), filename.end(), L'-', L' ');
    std::replace(filename.begin(), filename.end(), L'_', L' ');

    // Replace 'ã' with 'a'
    std::replace(filename.begin(), filename.end(), L'ã', L'a');

    // Capitalize the first letter of each word
    bool capitalizeNext = true;
    for (wchar_t& ch : filename) {
        if (capitalizeNext && std::iswalpha(ch)) {
            ch = std::towupper(ch);
            capitalizeNext = false;
        } else if (std::iswspace(ch)) {
            capitalizeNext = true;
        }
    }

    // Convert std::wstring back to UTF-8 std::string
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(filename);
}

