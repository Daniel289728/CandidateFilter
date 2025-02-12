#include "candidate.h"
#include <algorithm> // For std::find
#include <sstream>   // For converting vector to string

// Getters
const std::string& Candidate::getName() const {
    return name;
}

const std::string& Candidate::getUniversity() const {
    return university;
}

double Candidate::getGPA() const {
    return GPA;
}

const std::vector<std::string>& Candidate::getSkills() const {
    return skills;  // Return the skills vector by const reference
}

const std::string& Candidate::getHobby() const {
    return hobby;
}

double Candidate::getScore() const {
    return score;
}

// Convert the skills vector to a comma-separated string
std::string Candidate::getSkillsAsString() const {
    std::ostringstream oss;  // String stream to build the comma-separated string
    for (size_t i = 0; i < skills.size(); ++i) {
        oss << skills[i];
        if (i != skills.size() - 1) {
            oss << ", ";  // Add a separator if it's not the last skill
        }
    }
    return oss.str();  // Return the concatenated string
}

// Setters
void Candidate::setName(const std::string& _name) {
    name = _name;
}

void Candidate::setUniversity(const std::string& _university) {
    university = _university;
}

void Candidate::setGPA(double _gpa) {
    GPA = _gpa;
}

void Candidate::setHobby(const std::string& _hobby) {
    hobby = _hobby;
}

void Candidate::setSkills(const std::vector<std::string>& newSkills) {
    skills = newSkills;  // Replace the current skills list with a new one
}

void Candidate::setScore(double _score) {
    score = _score;
}

// Add a skill to the list, avoiding duplicates
void Candidate::addSkill(const std::string& newSkill) {
    // Avoid adding duplicate skills
    if (std::find(skills.begin(), skills.end(), newSkill) == skills.end()) {
        skills.push_back(newSkill);
    }
}

// Constructor that allows initializing with optional parameters
Candidate::Candidate(const std::string& _name, const std::string& _university, double _gpa,
                     const std::vector<std::string>& _skills, const std::string& _hobby, double _score)
    : name(_name), university(_university), GPA(_gpa), skills(_skills), hobby(_hobby), score(_score) {}

