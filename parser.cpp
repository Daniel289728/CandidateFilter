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
        Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
            QString("Failed to load XML file: %1. Error: %2")
                .arg(QString::fromStdString(filePath))
                .arg(result.description()),
            "Parser-XML");
        return candidates;
    }

    // Check if <root> node exists
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
            
            // Check for required fields
            if (!candidateNode.child("name")) {
                Logger::getInstance().log(Logger::LogLevel::LOG_WARNING,
                    QString("Missing name for candidate %1 in file %2")
                        .arg(candidateCount + 1)
                        .arg(QString::fromStdString(filePath)),
                    "Parser-XML");
                continue;
            }

            candidate.setName(candidateNode.child("name").text().as_string());

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

            pugi::xml_node skillsNode = candidateNode.child("skills");
            for (pugi::xml_node skill = skillsNode.child("skill"); skill; skill = skill.next_sibling("skill")) {
                candidate.addSkill(skill.text().as_string());
            }

            if (candidateNode.child("hobby")) {
                candidate.setHobby(candidateNode.child("hobby").text().as_string());
            }

            candidates.push_back(candidate);
            candidateCount++;
        } catch (const std::exception& e) {
            Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
                QString("Error processing candidate %1 in file %2: %3")
                    .arg(candidateCount + 1)
                    .arg(QString::fromStdString(filePath))
                    .arg(e.what()),
                "Parser-XML");
        }
    }

    Logger::getInstance().log(Logger::LogLevel::LOG_INFO,
        QString("Successfully parsed %1 candidates from XML file: %2")
            .arg(candidateCount)
            .arg(QString::fromStdString(filePath)),
        "Parser-XML");

    return candidates;
}

std::vector<Candidate> Parser::parseJSON(const std::string& filePath) {
    std::vector<Candidate> candidates;
    std::ifstream inputFile(filePath);

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
                if (!candidate.contains("name")) {
                    Logger::getInstance().log(Logger::LogLevel::LOG_WARNING,
                        QString("Missing name for candidate %1 in file %2")
                            .arg(candidateCount + 1)
                            .arg(QString::fromStdString(filePath)),
                        "Parser-JSON");
                    continue;
                }

                c.setName(candidate["name"]);

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

                if (candidate.contains("skills")) {
                    c.setSkills(candidate["skills"].get<std::vector<std::string>>());
                }

                if (candidate.contains("hobby")) {
                    c.setHobby(candidate["hobby"]);
                }

                candidates.push_back(c);
                candidateCount++;
            } catch (const std::exception& e) {
                Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
                    QString("Error processing candidate %1 in file %2: %3")
                        .arg(candidateCount + 1)
                        .arg(QString::fromStdString(filePath))
                        .arg(e.what()),
                    "Parser-JSON");
            }
        }

        Logger::getInstance().log(Logger::LogLevel::LOG_INFO,
            QString("Successfully parsed %1 candidates from JSON file: %2")
                .arg(candidateCount)
                .arg(QString::fromStdString(filePath)),
            "Parser-JSON");

    } catch (const nlohmann::json::exception& e) {
        Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
            QString("JSON parsing error in file %1: %2")
                .arg(QString::fromStdString(filePath))
                .arg(e.what()),
            "Parser-JSON");
    }

    return candidates;
}

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

            // Assign university name to each candidate
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

    Logger::getInstance().log(Logger::LogLevel::LOG_INFO,
        QString("Total candidates processed: %1")
            .arg(allCandidates.size()),
        "Parser");

    return allCandidates;
}

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
