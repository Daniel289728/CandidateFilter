#ifndef CANDIDATE_H
#define CANDIDATE_H
#include <string>
#include<vector>

// Class representing a candidate in a job application or university admission system
class Candidate {
private:
    std::string name;               // Name of the candidate
    std::string university;          // University name
    double GPA;                      // Grade Point Average (GPA)
    std::vector<std::string> skills; // List of skills the candidate possesses
    std::string hobby;               // Candidate's hobby
    double score;                    // Candidate's overall score

public:
    // Getter functions (const references to avoid unnecessary copying)
    const std::string& getName() const;
    const std::string& getUniversity() const;
    double getGPA() const;
    const std::vector<std::string>& getSkills() const;
    const std::string& getHobby() const;
    double getScore() const;
    
    // Utility function to get skills as a comma-separated string
    std::string getSkillsAsString() const;

    // Setter functions
    void setName(const std::string& _name);
    void setUniversity(const std::string& _university);
    void setGPA(double _gpa);
    void setHobby(const std::string& _hobby);
    void setSkills(const std::vector<std::string>& newSkills);
    void setScore(double _score);

    // Add a single skill to the candidate's skillset (duplicates are avoided)
    void addSkill(const std::string& newSkill);

    // Constructor with default values for initialization
    Candidate(const std::string& _name = "", const std::string& _university = "", double _gpa = 0.0,
              const std::vector<std::string>& _skills = {}, const std::string& _hobby = "", double _score = 0.0);
};

#endif // CANDIDATE_H
