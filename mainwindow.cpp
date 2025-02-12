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
    Downloader downloader;
    Parser parser;

    std::vector<std::string> rawData = downloader.downloadAllData();
    candidates = parser.processDownloadedData(rawData);
    CandidateScorer scorer;
    scorer.assignScores(candidates);
    displayCandidates(candidates);
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
        return;  // No file selected
    }

    QJsonArray candidatesArray;
    
    // Use currentlyDisplayedCandidates instead of all candidates
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

    QFile saveFile(fileName);
    if (saveFile.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(candidatesArray);
        saveFile.write(doc.toJson());
    }
}

void MainWindow::loadFilteredCandidates() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Filters", "", "JSON Files (*.json);;All Files (*)");
    if (fileName.isEmpty()) {
        return;  // No file selected
    }

    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Could not open file");
        return;
    }

    QByteArray data = loadFile.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonArray candidatesArray = doc.array();
    candidates.clear();

    for (const auto& item : candidatesArray) {
        QJsonObject obj = item.toObject();
        std::string name = obj["name"].toString().toStdString();
        std::string university = obj["university"].toString().toStdString();
        std::string hobby = obj["hobby"].toString().toStdString();
        double gpa = obj["gpa"].toDouble();
        double score = obj["score"].toDouble();

        // Convert skills from JSON array to std::vector<std::string>
        QJsonArray skillsArray = obj["skills"].toArray();
        std::vector<std::string> skills;
        for (const auto& skillItem : skillsArray) {
            skills.push_back(skillItem.toString().toStdString());
        }

        candidates.push_back(Candidate(name, university, gpa, skills, hobby, score));
    }

    displayCandidates(candidates);
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

