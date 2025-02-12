#include "candidateFilter.h"
#include <algorithm>

std::vector<Candidate> CandidateFilter::filterByGPA(const std::vector<Candidate>& candidates, double minGPA) {
    std::vector<Candidate> filtered;
    for (const auto& candidate : candidates) {
        if (candidate.getGPA() >= minGPA) {
            filtered.push_back(candidate);
        }
    }
    return filtered;
}

std::vector<Candidate> CandidateFilter::filterBySkill(const std::vector<Candidate>& candidates, const std::string& skill) {
    std::vector<Candidate> filtered;

    for (const auto& candidate : candidates) {
        if (std::find(candidate.getSkills().begin(), candidate.getSkills().end(), skill) != candidate.getSkills().end()) {
            filtered.push_back(candidate);
        }
    }
    return filtered;
}

std::vector<Candidate> CandidateFilter::filterByUniversity(const std::vector<Candidate>& candidates, const std::string& university) {
    std::vector<Candidate> filtered;
    for (const auto& candidate : candidates) {
        if (candidate.getUniversity() == university) {
            filtered.push_back(candidate);
        }
    }
    return filtered;
}
