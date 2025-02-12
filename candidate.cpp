#include "candidate.h"
#include <algorithm>

const std::string& Candidate::getName() const{
    return name;
}
const std::string& Candidate::getUniversity() const{
    return university;
}
double Candidate::getGPA() const{
    return GPA;
}
const std::vector<std::string>& Candidate::getSkills() const {
    return skills;  // return a const reference to the vector
}

const std::string& Candidate::getHobby() const{
    return hobby;
}
double Candidate::getScore() const{
    return score;
}
std::string Candidate::getSkillsAsString() const {
    std::string skillsStr;
    for (size_t i = 0; i < skills.size(); ++i) {
        skillsStr += skills[i];  // Add the skill
        if (i != skills.size() - 1) {
            skillsStr += ", ";  // Add a separator (comma and space)
        }
    }
    return skillsStr;
}
void Candidate::setName(const std::string& _name){
    name = _name;
}
void Candidate::setUniversity(const std::string& _uni){
    university = _uni;
}
void Candidate::setGPA(double _gpa){
    GPA = _gpa;
}
void Candidate::setHobby(const std::string& _hobby){
    hobby = _hobby;
}
void Candidate::setSkills(const std::vector<std::string>& newSkills) {
    skills = newSkills;  // Replace the existing skills vector with a new one
}
void Candidate::setScore(double sc){
    score = sc;
}
void Candidate::addSkill(const std::string& newSkill) {
    // Optionally, check for duplicates before adding
    if (std::find(skills.begin(), skills.end(), newSkill) == skills.end()) {
        skills.push_back(newSkill);
    }
}
