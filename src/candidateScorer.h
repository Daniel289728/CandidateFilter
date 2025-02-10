#ifndef CANDIDATESCORER_H
#define CANDIDATESCORER_H

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include "candidate.h"

class CandidateScorer {
public:
    // Constructor
    CandidateScorer();

    // Destructor
    ~CandidateScorer();

    // Method to assign scores to candidates
    void assignScores(std::vector<Candidate>& candidates);

private:
    // Method to calculate the score based on GPA and skills
    double calculateScore(const Candidate& candidate);
};
#endif