Candidate Data Visualization and Ranking Application
Overview
This C++ Windows application downloads and analyzes candidate data from four universities:

Polytechnic University of Bucharest
University of São Paulo
University of Florida
University of Havana
The application visualizes candidate information, calculates scores based on GPA, skills, and university, and provides features for filtering, saving, and reloading candidate lists. The data is refreshed daily, and error logging is provided to track issues in the data feeds.

Technologies Used
Qt: Framework for the graphical user interface (GUI).
PGUIXML: For parsing XML data.
nlohmann_json: For parsing JSON data.
CURL: For downloading candidate data from university URLs.
C++: Language for implementing the core logic and functionality.
Features
Data Download: Automatically downloads candidate data from the university URLs daily.
Data Analysis:
Total number of candidates.
Total number of candidates from each university.
Average GPA of candidates from each university.
Most common skills across all candidates.
Candidate View:
Display a list of all candidates with their GPA and skills.
Filter candidates by GPA range, university, or skills.
Ranking:
Implement a scoring system based on GPA, matching skills, and university weight.
Sort and display candidates by their scores.
Persistence:
Save filtered results to disk and load them without overwriting current day data.
Error Logging: Logs any errors triggered by the candidate feed to a file for troubleshooting.
Scoring Function Example
The candidate score is calculated using the following formula:

ini
Copy
Edit
Score = (GPA × GPA Weight) + (Matching Skills × Skill Weight) + University Weight
Weights can be adjusted as per your preference to prioritize certain criteria.

Setup and Installation
Prerequisites
Qt: Install Qt 5.15 or later.
CMake: Required for building the application.
CURL: Install the CURL library.
nlohmann_json: Available as a header-only library.
PGUIXML: For XML parsing, ensure you have the necessary libraries installed.
Building the Application
Clone the repository:

bash
Copy
Edit
git clone <repository_link>
Navigate to the project folder:

bash
Copy
Edit
cd <project_folder>
Create a build directory:

bash
Copy
Edit
mkdir build
cd build
Generate the build files with CMake:

bash
Copy
Edit
cmake ..
Build the application:

bash
Copy
Edit
cmake --build .
After building, you should find the executable in the build directory.

Running the Application
To run the application, simply execute the generated executable:

bash
Copy
Edit
./candidate_visualizer
Usage
The application will automatically download the candidate data daily from the university URLs.
The main window will display the candidate list, with filtering options for GPA range, university, and skills.
You can sort the candidates by score, which is calculated based on GPA, matching skills, and university weight.
You can save filtered candidate lists to disk and load them later without overwriting the current day's data.
Errors in the data feed will be logged in a separate file, which can be used for troubleshooting.
Error Logging
All errors triggered by the candidate feed are logged in a file named error_log.txt in the application's directory. This will help track issues such as incorrect data formats, missing fields, or other data-related problems.

Conclusion
This application is designed to help Eightpoint identify the best candidates by automating the download, analysis, ranking, and filtering of graduate student data from various universities. The system can be easily extended or modified to incorporate additional features or handle additional universities' data feeds.