#include <fstream>
#include <algorithm>
#include <string>
#include "parser.h"
#include "pugixml.hpp"
#include "logger.h"

Parser::Parser() {
    // Constructor body can be used for initialization, if needed
}

Parser::~Parser() {
    // Destructor body can be used for resource cleanup, if necessary
}

// Function to parse XML file and extract candidate data
std::vector<Candidate> Parser::parseXML(const std::string& filePath) {
    std::vector<Candidate> candidates;
    pugi::xml_document doc;

    // Load XML file and check for errors
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());
    if (!result) {
        Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
            QString("Failed to load XML file: %1. Error: %2")
                .arg(QString::fromStdString(filePath))
                .arg(result.description()),
            "Parser-XML");
        return candidates;  // Return empty vector on failure
    }

    // Find root node
    pugi::xml_node rootNode = doc.child("root");
    if (!rootNode) {
        Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
            QString("Missing <root> node in XML file: %1")
                .arg(QString::fromStdString(filePath)),
            "Parser-XML");
        return candidates;
    }

    int candidateCount = 0;
    for (pugi::xml_node candidateNode = rootNode.child("candidate"); candidateNode;
         candidateNode = candidateNode.next_sibling("candidate")) {
        try {
            Candidate candidate;
            
            // Validate required fields and parse data
            if (!candidateNode.child("name")) {
                Logger::getInstance().log(Logger::LogLevel::LOG_WARNING,
                    QString("Missing name for candidate %1 in file %2")
                        .arg(candidateCount + 1)
                        .arg(QString::fromStdString(filePath)),
                    "Parser-XML");
                continue;
            }
            candidate.setName(candidateNode.child("name").text().as_string());

            // Handle missing GPA
            if (!candidateNode.child("GPA")) {
                Logger::getInstance().log(Logger::LogLevel::LOG_WARNING,
                    QString("Missing GPA for candidate %1 (%2) in file %3")
                        .arg(candidateCount + 1)
                        .arg(QString::fromStdString(candidate.getName()))
                        .arg(QString::fromStdString(filePath)),
                    "Parser-XML");
            } else {
                candidate.setGPA(candidateNode.child("GPA").text().as_double());
            }

            // Parse skills and hobbies
            parseSkills(candidate, candidateNode);
            parseHobby(candidate, candidateNode);

            candidates.push_back(candidate);
            candidateCount++;
        } catch (const std::exception& e) {
            // Handle exception if parsing a candidate fails
            Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
                QString("Error processing candidate %1 in file %2: %3")
                    .arg(candidateCount + 1)
                    .arg(QString::fromStdString(filePath))
                    .arg(e.what()),
                "Parser-XML");
        }
    }

    // Log successful parsing
    Logger::getInstance().log(Logger::LogLevel::LOG_INFO,
        QString("Successfully parsed %1 candidates from XML file: %2")
            .arg(candidateCount)
            .arg(QString::fromStdString(filePath)),
        "Parser-XML");

    return candidates;
}

// Helper function to parse skills of the candidate from the XML
void Parser::parseSkills(Candidate& candidate, const pugi::xml_node& candidateNode) {
    pugi::xml_node skillsNode = candidateNode.child("skills");
    for (pugi::xml_node skill = skillsNode.child("skill"); skill; skill = skill.next_sibling("skill")) {
        candidate.addSkill(skill.text().as_string());
    }
}

// Helper function to parse the hobby of the candidate from the XML
void Parser::parseHobby(Candidate& candidate, const pugi::xml_node& candidateNode) {
    if (candidateNode.child("hobby")) {
        candidate.setHobby(candidateNode.child("hobby").text().as_string());
    }
}

// Function to parse JSON file and extract candidate data
std::vector<Candidate> Parser::parseJSON(const std::string& filePath) {
    std::vector<Candidate> candidates;
    std::ifstream inputFile(filePath);

    // Check if file is open
    if (!inputFile.is_open()) {
        Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
            QString("Failed to open JSON file: %1")
                .arg(QString::fromStdString(filePath)),
            "Parser-JSON");
        return candidates;
    }

    try {
        nlohmann::json jsonData;
        inputFile >> jsonData;

        int candidateCount = 0;
        for (const auto& candidate : jsonData) {
            try {
                Candidate c;
                
                // Handle missing "name" field in JSON
                if (!candidate.contains("name")) {
                    Logger::getInstance().log(Logger::LogLevel::LOG_WARNING,
                        QString("Missing name for candidate %1 in file %2")
                            .arg(candidateCount + 1)
                            .arg(QString::fromStdString(filePath)),
                        "Parser-JSON");
                    continue;
                }

                c.setName(candidate["name"]);

                // Handle missing GPA
                if (!candidate.contains("GPA")) {
                    Logger::getInstance().log(Logger::LogLevel::LOG_WARNING,
                        QString("Missing GPA for candidate %1 (%2) in file %3")
                            .arg(candidateCount + 1)
                            .arg(QString::fromStdString(c.getName()))
                            .arg(QString::fromStdString(filePath)),
                        "Parser-JSON");
                } else {
                    c.setGPA(candidate["GPA"]);
                }

                // Parse skills and hobbies
                if (candidate.contains("skills")) {
                    c.setSkills(candidate["skills"].get<std::vector<std::string>>());
                }

                if (candidate.contains("hobby")) {
                    c.setHobby(candidate["hobby"]);
                }

                candidates.push_back(c);
                candidateCount++;
            } catch (const std::exception& e) {
                // Handle exception if parsing a candidate fails
                Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
                    QString("Error processing candidate %1 in file %2: %3")
                        .arg(candidateCount + 1)
                        .arg(QString::fromStdString(filePath))
                        .arg(e.what()),
                    "Parser-JSON");
            }
        }

        // Log successful parsing
        Logger::getInstance().log(Logger::LogLevel::LOG_INFO,
            QString("Successfully parsed %1 candidates from JSON file: %2")
                .arg(candidateCount)
                .arg(QString::fromStdString(filePath)),
            "Parser-JSON");

    } catch (const nlohmann::json::exception& e) {
        // Handle JSON parsing errors
        Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
            QString("JSON parsing error in file %1: %2")
                .arg(QString::fromStdString(filePath))
                .arg(e.what()),
            "Parser-JSON");
    }

    return candidates;
}

// Main function to process all downloaded data files
std::vector<Candidate> Parser::processDownloadedData(const std::vector<std::string>& dataFiles) {
    std::vector<Candidate> allCandidates;

    for (const auto& filePath : dataFiles) {
        Logger::getInstance().log(Logger::LogLevel::LOG_INFO,
            QString("Processing file: %1")
                .arg(QString::fromStdString(filePath)),
            "Parser");

        try {
            std::string universityName = extractUniversityName(filePath);
            Logger::getInstance().log(Logger::LogLevel::LOG_INFO,
                QString("Extracted university name: %1")
                    .arg(QString::fromStdString(universityName)),
                "Parser");

            std::vector<Candidate> candidates;
            std::string extension = filePath.substr(filePath.find_last_of(".") + 1);

            // Check file extension and parse accordingly
            if (extension == "xml") {
                candidates = parseXML(filePath);
            } else if (extension == "json") {
                candidates = parseJSON(filePath);
            } else {
                Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
                    QString("Unsupported file format: %1")
                        .arg(QString::fromStdString(extension)),
                    "Parser");
                continue;
            }

            // Assign university name to candidates
            for (auto& candidate : candidates) {
                candidate.setUniversity(universityName);
            }

            allCandidates.insert(allCandidates.end(), candidates.begin(), candidates.end());

        } catch (const std::exception& e) {
            Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
                QString("Error processing file %1: %2")
                    .arg(QString::fromStdString(filePath))
                    .arg(e.what()),
                "Parser");
        }
    }

    // Log the total candidates processed
    Logger::getInstance().log(Logger::LogLevel::LOG_INFO,
        QString("Total candidates processed: %1")
            .arg(allCandidates.size()),
        "Parser");

    return allCandidates;
}

// Function to extract university name from the file path
std::string Parser::extractUniversityName(const std::string& filePath) {
    std::filesystem::path path(filePath);
    
    // Convert to wstring for Unicode handling
    std::wstring filename = path.stem().wstring();

    // Replace characters to clean filename
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

    // Convert back to UTF-8 std::string
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(filename);
}
