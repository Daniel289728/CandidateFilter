#include "candidateScorer.h"


void CandidateScorer::assignScores(std::vector<Candidate>& candidates) {
    for (auto& candidate : candidates) {
        candidate.setScore(calculateScore(candidate));  // Calculate score for each candidate
    }
}
double CandidateScorer::calculateScore(const Candidate& candidate) {
    double score = 0.0;

    // GPA weight: Assume GPA is out of 4.0
    if (candidate.getGPA() >= 3.5) {
        score += 50.0;  // High GPA
    }
    else if (candidate.getGPA() >= 2.5) {
        score += 30.0;  // Average GPA
    }
    else {
        score += 10.0;  // Low GPA
    }

    // Skills weight: Assume each skill adds a certain score
    if (!candidate.getSkills().empty()) {
        score += 10.0 * candidate.getSkills().size();  // Add 10 points per skill
    }

    // Hobby weight: Assume hobby adds a small bonus
    if (!candidate.getHobby().empty()) {
        score += 5.0;  // Bonus for having a hobby
    }

    return score;
}
