#ifndef CANDIDATESCORER_H
#define CANDIDATESCORER_H

#include <vector>
#include "candidate.h"

class CandidateScorer {
public:
    // Constructor
    CandidateScorer() {}

    // Destructor
    ~CandidateScorer() {}

    // Method to assign scores to all candidates
    void assignScores(std::vector<Candidate>& candidates);
    void assignScore(Candidate& candidate);
private:
    // Method to calculate the score based on GPA and skills
    double calculateScore(const Candidate& candidate) const;
};
#endif
