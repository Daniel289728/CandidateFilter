#ifndef CANDIDATE_H
#define CANDIDATE_H
#include <string>
#include<vector>
#include<iostream>

class Candidate {
public:
    std::string name;
    std::string university;
    double GPA;
    std::vector<std::string> skills;
    std::string hobby;
    double score;

    Candidate(const std::string& _name = "", const std::string& _university = "", double _gpa = 0.0,
              const std::vector<std::string>& _skills = {}, const std::string& _hobby = "")
        : name(_name), university(_university), GPA(_gpa), skills(_skills), hobby(_hobby) {}
    void print(void) const;
};
#endif
