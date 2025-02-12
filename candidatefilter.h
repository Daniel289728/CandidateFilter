#ifndef CANDIDATE_FILTER_H
#define CANDIDATE_FILTER_H

#include "candidate.h"
#include <vector>

// Class that provides filtering operations for candidate lists based on different criteria
class CandidateFilter {
public:
    // Filter candidates by a minimum GPA
    static std::vector<Candidate> filterByGPA(const std::vector<Candidate>& candidates, double minGPA);

    // Filter candidates by a specific skill
    static std::vector<Candidate> filterBySkill(const std::vector<Candidate>& candidates, const std::string& skill);

    // Filter candidates by university
    static std::vector<Candidate> filterByUniversity(const std::vector<Candidate>& candidates, const std::string& university);

    // Filter candidates by a minimum score
    static std::vector<Candidate> filterByScore(const std::vector<Candidate>& candidates, double minScore);
};

#endif // CANDIDATE_FILTER_H
