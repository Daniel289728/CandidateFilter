#ifndef CANDIDATE_FILTER_H
#define CANDIDATE_FILTER_H

#include "candidate.h"
#include <vector>
class CandidateFilter {
public:
    static std::vector<Candidate> filterByGPA(const std::vector<Candidate>& candidates, double minGPA);
    static std::vector<Candidate> filterBySkill(const std::vector<Candidate>& candidates, const std::string& skill);
    static std::vector<Candidate> filterByUniversity(const std::vector<Candidate>& candidates, const std::string& university);
};

#endif
