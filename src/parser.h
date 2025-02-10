#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>  // For JSON parsing
#include <pugixml.hpp>        // For XML parsing

class Parser
{
public:
	Parser();
	~Parser();
	void parseXML(const std::string& filePath);
	void parseJSON(const std::string& filePath);
	void processDownloadedData(const std::vector<std::string>& dataFiles);
};


#endif // PARSER_H
