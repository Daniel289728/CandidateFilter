#include "candidateFilter.h"
#include <algorithm> // For std::find

// Filter candidates by a minimum GPA
std::vector<Candidate> CandidateFilter::filterByGPA(const std::vector<Candidate>& candidates, double minGPA) {
    std::vector<Candidate> filtered;
    
    // Iterate over candidates and select those who meet the GPA requirement
    for (const auto& candidate : candidates) {
        if (candidate.getGPA() >= minGPA) {
            filtered.push_back(candidate);
        }
    }
    
    return filtered;
}

// Filter candidates by a specific skill
std::vector<Candidate> CandidateFilter::filterBySkill(const std::vector<Candidate>& candidates, const std::string& skill) {
    std::vector<Candidate> filtered;

    // Iterate over candidates and select those who have the specified skill
    for (const auto& candidate : candidates) {
        if (std::find(candidate.getSkills().begin(), candidate.getSkills().end(), skill) != candidate.getSkills().end()) {
            filtered.push_back(candidate);
        }
    }

    return filtered;
}

// Filter candidates by university name
std::vector<Candidate> CandidateFilter::filterByUniversity(const std::vector<Candidate>& candidates, const std::string& university) {
    std::vector<Candidate> filtered;

    // Iterate over candidates and select those who belong to the specified university
    for (const auto& candidate : candidates) {
        if (candidate.getUniversity() == university) {
            filtered.push_back(candidate);
        }
    }

    return filtered;
}

// Filter candidates by a minimum score
std::vector<Candidate> CandidateFilter::filterByScore(const std::vector<Candidate>& candidates, double minScore) {
    std::vector<Candidate> filtered;

    // Iterate over candidates and select those who meet the score requirement
    for (const auto& candidate : candidates) {
        if (candidate.getScore() >= minScore) {
            filtered.push_back(candidate);
        }
    }

    return filtered;
}
