#ifndef CANDIDATE_H
#define CANDIDATE_H
#include <string>
#include<vector>

class Candidate {
private:
    std::string name;
    std::string university;
    double GPA;
    std::vector<std::string> skills;
    std::string hobby;
    double score;
public:
    const std::string& getName() const;
    const std::string& getUniversity() const;
    double getGPA() const;
    const std::vector<std::string>& getSkills() const;
    const std::string& getHobby() const;
    double getScore()const;
    std::string getSkillsAsString() const;

    void setName(const std::string& _name);
    void setUniversity(const std::string& _uni);
    void setGPA(double _gpa);
    void setHobby(const std::string& _hobby);
    void setSkills(const std::vector<std::string>& newSkills);
    void setScore(double sc);
    void addSkill(const std::string& newSkill);

    Candidate(const std::string& _name = "", const std::string& _university = "", double _gpa = 0.0,
              const std::vector<std::string>& _skills = {}, const std::string& _hobby = "", double _score = 0.0)
        : name(_name), university(_university), GPA(_gpa), skills(_skills), hobby(_hobby), score(_score) {}
};
#endif
