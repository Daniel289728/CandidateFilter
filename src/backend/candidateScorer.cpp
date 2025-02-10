#include "candidateScorer.h"


void CandidateScorer::assignScores(std::vector<Candidate>& candidates) {
    for (auto& candidate : candidates) {
        candidate.score = calculateScore(candidate);  // Calculate score for each candidate
    }
}
double CandidateScorer::calculateScore(const Candidate& candidate) {
    double score = 0.0;

    // GPA weight: Assume GPA is out of 4.0
    if (candidate.GPA >= 3.5) {
        score += 50.0;  // High GPA
    }
    else if (candidate.GPA >= 2.5) {
        score += 30.0;  // Average GPA
    }
    else {
        score += 10.0;  // Low GPA
    }

    // Skills weight: Assume each skill adds a certain score
    if (!candidate.skills.empty()) {
        score += 10.0 * candidate.skills.size();  // Add 10 points per skill
    }

    // Hobby weight: Assume hobby adds a small bonus
    if (!candidate.hobby.empty()) {
        score += 5.0;  // Bonus for having a hobby
    }

    return score;
}