#ifndef CANDIDATESCORER_H
#define CANDIDATESCORER_H

#include <vector>
#include "candidate.h"

// Class that assigns scores to candidates based on different criteria like GPA, skills, and hobbies
class CandidateScorer {
public:
    // Constructor
    CandidateScorer() = default;

    // Destructor
    ~CandidateScorer() = default;

    // Method to assign scores to all candidates
    void assignScores(std::vector<Candidate>& candidates);

    // Method to assign a score to a single candidate
    void assignScore(Candidate& candidate);

private:
    // Method to calculate the score based on GPA, skills, and hobby
    double calculateScore(const Candidate& candidate) const;
};

#endif // CANDIDATE_SCORER_H
