#include "MainWindow.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QHeaderView>
#include <QMap>
#include <QSet>
#include <QFile>
#include <QTextStream>
#include "Logger.h"
#include <stdexcept>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      candidateTableView(new QTableView(this)),
      candidateModel(new QStandardItemModel(this)),
      summaryTextEdit(new QTextEdit(this)),
      stackedWidget(new QStackedWidget(this)),
      filterButton(new QPushButton("Apply Filters", this)),
      saveButton(new QPushButton("Save Filters", this)),
      loadButton(new QPushButton("Load Filters", this)),
      rankButton(new QPushButton("Rank Candidates", this)),
      summaryButton(new QPushButton("View Summary", this)),
      toggleViewButton(new QPushButton("Show Full List", this)),
      filterComboBox(new QComboBox(this)),
      universityComboBox(new QComboBox(this)),
      skillsComboBox(new QComboBox(this)),
      gpaRangeInput(new QLineEdit(this)),
      scoreInput(new QLineEdit(this)),
      gpaRangeLabel(new QLabel("Enter Min GPA:", this)),
      scoreLabel(new QLabel("Enter Min Score:", this)),
      layout(new QVBoxLayout()),
      centralWidget(new QWidget(this)),
      isSummaryView(false)
{
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    candidateModel->setHorizontalHeaderLabels({"Name", "University", "GPA", "Skills", "Score"});
    candidateTableView->setModel(candidateModel);
    candidateTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Set up the stacked widget
    stackedWidget->addWidget(candidateTableView);
    stackedWidget->addWidget(summaryTextEdit);
    summaryTextEdit->setReadOnly(true);  // Make it read-only
    
    // Add widgets to layout
    layout->addWidget(stackedWidget);
    layout->addWidget(filterComboBox);
    layout->addWidget(gpaRangeLabel);
    layout->addWidget(gpaRangeInput);
    layout->addWidget(scoreLabel);
    layout->addWidget(scoreInput);
    layout->addWidget(universityComboBox);
    layout->addWidget(skillsComboBox);
    layout->addWidget(filterButton);
    layout->addWidget(saveButton);
    layout->addWidget(loadButton);
    layout->addWidget(toggleViewButton);

    // Initially hide the filter elements
    universityComboBox->setVisible(false);
    skillsComboBox->setVisible(false);
    gpaRangeInput->setVisible(false);
    gpaRangeLabel->setVisible(false);
    scoreInput->setVisible(false);
    scoreLabel->setVisible(false);
    filterComboBox->setVisible(false);

    // Populate filterComboBox with options
    filterComboBox->addItems({"No Filter", "GPA Range", "University", "Skills", "Score"});

    connect(filterButton, &QPushButton::clicked, this, &MainWindow::applyFilters);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveFilteredCandidates);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadFilteredCandidates);
    connect(toggleViewButton, &QPushButton::clicked, this, &MainWindow::toggleView);
    connect(filterComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onFilterChanged);

    // Populate ComboBoxes with sample data
    universityComboBox->addItems({"University Of Sao Paulo", "University Of Florida", "University Of Havana", "Polytechnic University Of Bucharest"});
    skillsComboBox->addItems({"C++", "Python", "Java", "C#"});

    loadCandidates();
    toggleView();  // Show summary view by default
}

MainWindow::~MainWindow() {}

void MainWindow::loadCandidates() {
    try {
        Downloader downloader;
        Parser parser;

        std::vector<std::string> rawData;
        try {
            rawData = downloader.downloadAllData();
        } catch (const std::exception& e) {
            Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
                QString("Failed to download candidate data: %1").arg(e.what()), 
                "Downloader");
            throw; // Re-throw to be caught by outer try-catch
        }

        try {
            candidates = parser.processDownloadedData(rawData);
        } catch (const std::exception& e) {
            Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
                QString("Failed to parse candidate data: %1").arg(e.what()), 
                "Parser");
            throw;
        }

        CandidateScorer scorer;
        try {
            scorer.assignScores(candidates);
        } catch (const std::exception& e) {
            Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
                QString("Failed to assign scores: %1").arg(e.what()), 
                "Scorer");
            throw;
        }

        displayCandidates(candidates);
        Logger::getInstance().log(Logger::LogLevel::LOG_INFO,
            QString("Successfully loaded %1 candidates").arg(candidates.size()), 
            "MainWindow");
    } catch (const std::exception& e) {
        Logger::getInstance().log(Logger::LogLevel::LOG_ERROR,
            QString("Fatal error in loadCandidates: %1").arg(e.what()), 
            "MainWindow");
        QMessageBox::critical(this, "Error", "Failed to load candidates. Check error log for details.");
    }
}

void MainWindow::displayCandidates(const std::vector<Candidate>& candidatesToShow) {
    currentlyDisplayedCandidates = candidatesToShow;  // Store the current view
    candidateModel->setRowCount(0);
    for (const auto& c : candidatesToShow) {
        QList<QStandardItem*> row;
        row.append(new QStandardItem(QString::fromStdString(c.getName())));
        row.append(new QStandardItem(QString::fromStdString(c.getUniversity())));
        row.append(new QStandardItem(QString::number(c.getGPA())));
        row.append(new QStandardItem(QString::fromStdString(c.getSkillsAsString())));
        row.append(new QStandardItem(QString::number(c.getScore())));
        candidateModel->appendRow(row);
    }
}

void MainWindow::generateSummary() {
    QString summaryText;
    QTextStream stream(&summaryText);
    
    int totalCandidates = candidates.size();
    QMap<QString, int> universityCount;
    QMap<QString, double> universityGPA;
    QMap<QString, int> skillCount;

    for (const auto& c : candidates) {
        universityCount[QString::fromStdString(c.getUniversity())]++;
        universityGPA[QString::fromStdString(c.getUniversity())] += c.getGPA();
        for (const auto& skill : c.getSkills()) {
            skillCount[QString::fromStdString(skill)]++;
        }
    }

    // Create formatted summary text
    stream << "<h2>Candidate Summary Report</h2>\n\n";
    stream << "<p><b>Total Candidates:</b> " << totalCandidates << "</p>\n\n";
    
    stream << "<h3>University Statistics:</h3>\n";
    for (auto it = universityCount.begin(); it != universityCount.end(); ++it) {
        double avgGPA = universityGPA[it.key()] / it.value();
        stream << "<p>" << it.key() << ":<br>\n";
        stream << "   Candidates: " << it.value() << "<br>\n";
        stream << "   Average GPA: " << QString::number(avgGPA, 'f', 2) << "</p>\n";
    }
    
    stream << "<h3>Top 5 Skills:</h3>\n<p>";
    QVector<QPair<QString, int>> sortedSkills;
    for (auto it = skillCount.begin(); it != skillCount.end(); ++it) {
        sortedSkills.append(qMakePair(it.key(), it.value()));
    }
    
    // Sort by count in descending order
    std::sort(sortedSkills.begin(), sortedSkills.end(), 
        [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
            return a.second > b.second;
        });
    
    // Show only top 5 skills
    int count = 0;
    for (const auto& skill : sortedSkills) {
        if (count >= 5) break;
        stream << skill.first << ": " << skill.second << " candidates<br>\n";
        count++;
    }
    stream << "</p>";

    summaryTextEdit->setHtml(summaryText);
}

void MainWindow::toggleView() {
    isSummaryView = !isSummaryView;
    if (isSummaryView) {
        generateSummary();
        stackedWidget->setCurrentWidget(summaryTextEdit);
        toggleViewButton->setText("Show Full List");
        filterButton->setVisible(false);
        saveButton->setVisible(false);
        filterComboBox->setVisible(false);
        universityComboBox->setVisible(false);
        skillsComboBox->setVisible(false);
        gpaRangeInput->setVisible(false);
        gpaRangeLabel->setVisible(false);
        scoreInput->setVisible(false);
        scoreLabel->setVisible(false);
    } else {
        stackedWidget->setCurrentWidget(candidateTableView);
        displayCandidates(candidates);
        toggleViewButton->setText("Show Summary");
        filterButton->setVisible(true);
        saveButton->setVisible(true);
        loadButton->setVisible(true);
        filterComboBox->setVisible(true);
    }
}

void MainWindow::applyFilters() {
    std::vector<Candidate> filteredCandidates = candidates;  // Start with all candidates
    // Filter by GPA range
    if (filterComboBox->currentText() == "GPA Range") {
        bool ok;
        double minGPA = gpaRangeInput->text().toDouble(&ok);
        if (ok) {
            filteredCandidates = CandidateFilter::filterByGPA(filteredCandidates, minGPA);
        }
    }

    // Filter by Score
    if (filterComboBox->currentText() == "Score") {
        bool ok;
        double minScore = scoreInput->text().toDouble(&ok);
        if (ok) {
            filteredCandidates = CandidateFilter::filterByScore(filteredCandidates, minScore);
        }
    }

    // Filter by University
    if (filterComboBox->currentText() == "University" && !universityComboBox->currentText().isEmpty()) {
        filteredCandidates = CandidateFilter::filterByUniversity(filteredCandidates, universityComboBox->currentText().toStdString());
    }

    // Filter by Skills
    if (filterComboBox->currentText() == "Skills" && !skillsComboBox->currentText().isEmpty()) {
        filteredCandidates = CandidateFilter::filterBySkill(filteredCandidates, skillsComboBox->currentText().toStdString());
    }

    displayCandidates(filteredCandidates);  // Update the table view
}

void MainWindow::saveFilteredCandidates() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Filters", "", "JSON Files (*.json);;All Files (*)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile saveFile(fileName);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        QString errorMsg = QString("Could not open file for writing: %1").arg(fileName);
        Logger::getInstance().log(Logger::LogLevel::LOG_ERROR, errorMsg, "FileOperation");
        QMessageBox::warning(this, "Error", "Could not save file");
        return;
    }

    try {
        QJsonArray candidatesArray;
        for (const auto& candidate : currentlyDisplayedCandidates) {
            QJsonObject candidateObject;
            candidateObject["name"] = QString::fromStdString(candidate.getName());
            candidateObject["university"] = QString::fromStdString(candidate.getUniversity());
            candidateObject["gpa"] = candidate.getGPA();
            candidateObject["score"] = candidate.getScore();

            QVector<QString> skillsVector;
            for (const auto& skill : candidate.getSkills()) {
                skillsVector.append(QString::fromStdString(skill));
            }
            candidateObject["skills"] = QJsonArray::fromStringList(skillsVector);

            candidatesArray.append(candidateObject);
        }

        QJsonDocument doc(candidatesArray);
        saveFile.write(doc.toJson());
        
        Logger::getInstance().log(Logger::LogLevel::LOG_INFO,
            QString("Successfully saved %1 candidates to file").arg(currentlyDisplayedCandidates.size()), 
            "FileOperation");
    } catch (const std::exception& e) {
        QString errorMsg = QString("Error saving candidates: %1").arg(e.what());
        Logger::getInstance().log(Logger::LogLevel::LOG_ERROR, errorMsg, "FileOperation");
        QMessageBox::warning(this, "Error", "Error saving candidates. Check error log for details.");
    }
}

void MainWindow::loadFilteredCandidates() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Filters", "", "JSON Files (*.json);;All Files (*)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        QString errorMsg = QString("Could not open file: %1").arg(fileName);
        Logger::getInstance().log(Logger::LogLevel::LOG_ERROR, errorMsg, "FileOperation");
        QMessageBox::warning(this, "Error", "Could not open file");
        return;
    }

    try {
        QByteArray data = loadFile.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(data));
        
        if (doc.isNull()) {
            QString errorMsg = QString("Invalid JSON format in file: %1").arg(fileName);
            Logger::getInstance().log(Logger::LogLevel::LOG_ERROR, errorMsg, "JSONParsing");
            QMessageBox::warning(this, "Error", "Invalid file format");
            return;
        }

        QJsonArray candidatesArray = doc.array();
        candidates.clear();

        for (int i = 0; i < candidatesArray.size(); ++i) {
            try {
                QJsonObject obj = candidatesArray[i].toObject();
                
                // Validate required fields
                if (!obj.contains("name") || !obj.contains("university") || 
                    !obj.contains("gpa") || !obj.contains("skills")) {
                    QString errorMsg = QString("Missing required fields in candidate at index %1").arg(i);
                    Logger::getInstance().log(Logger::LogLevel::LOG_ERROR, errorMsg, "DataValidation");
                    continue; // Skip this candidate
                }

                std::string name = obj["name"].toString().toStdString();
                std::string university = obj["university"].toString().toStdString();
                double gpa = obj["gpa"].toDouble();
                
                // Validate GPA range
                if (gpa < 0.0 || gpa > 4.0) {
                    QString warnMsg = QString("Invalid GPA value (%1) for candidate: %2")
                        .arg(gpa)
                        .arg(QString::fromStdString(name));
                    Logger::getInstance().log(Logger::LogLevel::LOG_WARNING, warnMsg, "DataValidation");
                }

                std::string hobby = obj["hobby"].toString().toStdString();
                double score = obj["score"].toDouble();

                QJsonArray skillsArray = obj["skills"].toArray();
                std::vector<std::string> skills;
                for (const auto& skillItem : skillsArray) {
                    skills.push_back(skillItem.toString().toStdString());
                }

                candidates.push_back(Candidate(name, university, gpa, skills, hobby, score));
            } catch (const std::exception& e) {
                QString errorMsg = QString("Error processing candidate at index %1: %2")
                    .arg(i)
                    .arg(e.what());
                Logger::getInstance().log(Logger::LogLevel::LOG_ERROR, errorMsg, "DataProcessing");
            }
        }

        displayCandidates(candidates);
        Logger::getInstance().log(Logger::LogLevel::LOG_INFO,
            QString("Successfully loaded %1 candidates from file").arg(candidates.size()), 
            "FileOperation");
    } catch (const std::exception& e) {
        QString errorMsg = QString("Error loading filtered candidates: %1").arg(e.what());
        Logger::getInstance().log(Logger::LogLevel::LOG_ERROR, errorMsg, "FileProcessing");
        QMessageBox::warning(this, "Error", "Error loading candidates. Check error log for details.");
    }
}

void MainWindow::rankCandidates() {
    std::sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
        return a.getScore() > b.getScore();  // Higher score comes first
    });

    displayCandidates(candidates);  // Display the ranked candidates
}

void MainWindow::onFilterChanged(const QString& text) {
    universityComboBox->setVisible(text == "University");
    skillsComboBox->setVisible(text == "Skills");
    gpaRangeInput->setVisible(text == "GPA Range");
    gpaRangeLabel->setVisible(text == "GPA Range");
    scoreInput->setVisible(text == "Score");
    scoreLabel->setVisible(text == "Score");
}

