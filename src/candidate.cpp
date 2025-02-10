#include "candidate.h"

void Candidate::print(void) const {
    std::cout << "Name: " << name << "\nUniversity: " << university << "\nGPA: " << GPA << "\nSkills: ";
    for (const auto& skill : skills) std::cout << skill << " ";
    std::cout << "\nHobby: " << hobby << "\n";
}