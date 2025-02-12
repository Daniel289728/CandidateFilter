#include "MainWindow.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), candidateTableView(new QTableView(this)),
    candidateModel(new QStandardItemModel(this)), filterButton(new QPushButton("Apply Filters", this)),
    saveButton(new QPushButton("Save Filters", this)), loadButton(new QPushButton("Load Filters", this)),
    rankButton(new QPushButton("Rank Candidates", this)), universityComboBox(new QComboBox(this)),
    skillsComboBox(new QComboBox(this)), gpaRangeCheckBox(new QCheckBox("Apply GPA Range", this)),
    layout(new QVBoxLayout()), centralWidget(new QWidget(this)) {

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    candidateModel->setHorizontalHeaderLabels({"Name", "University", "GPA", "Skills", "Score"});
    candidateTableView->setModel(candidateModel);
    candidateTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    layout->addWidget(candidateTableView);
    layout->addWidget(universityComboBox);
    layout->addWidget(skillsComboBox);
    layout->addWidget(gpaRangeCheckBox);
    layout->addWidget(filterButton);
    layout->addWidget(saveButton);
    layout->addWidget(loadButton);
    layout->addWidget(rankButton);

    connect(filterButton, &QPushButton::clicked, this, &MainWindow::applyFilters);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveFilteredCandidates);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadFilteredCandidates);
    connect(rankButton, &QPushButton::clicked, this, &MainWindow::rankCandidates);

    // Load candidate data on startup
    loadCandidates();
}

MainWindow::~MainWindow() {}

void MainWindow::loadCandidates() {
    Downloader downloader;
    Parser parser;

    std::vector<std::string> rawData = downloader.downloadAllData();
    candidates = parser.processDownloadedData(rawData);

    displayCandidates(candidates);
}

void MainWindow::displayCandidates(const std::vector<Candidate>& candidatesToShow) {
    candidateModel->setRowCount(0);  // Clear the table
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

void MainWindow::applyFilters() {
    std::vector<Candidate> filteredCandidates = candidates;

    if (gpaRangeCheckBox->isChecked()) {
        filteredCandidates = CandidateFilter::filterByGPA(filteredCandidates, 3.9);
    }

    if (!universityComboBox->currentText().isEmpty()) {
        filteredCandidates = CandidateFilter::filterByUniversity(filteredCandidates, universityComboBox->currentText().toStdString());
    }

    if (!skillsComboBox->currentText().isEmpty()) {
        filteredCandidates = CandidateFilter::filterBySkill(filteredCandidates, skillsComboBox->currentText().toStdString());
    }

    displayCandidates(filteredCandidates);
}

void MainWindow::rankCandidates() {
    std::sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
        return a.getScore() > b.getScore();  // Higher score first
    });
    displayCandidates(candidates);
}

void MainWindow::saveFilteredCandidates() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Filtered List", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) return;

    QJsonArray jsonArray;
    for (const auto& c : candidates) {
        QJsonObject json;
        json["name"] = QString::fromStdString(c.getName());
        json["university"] = QString::fromStdString(c.getUniversity());
        json["gpa"] = c.getGPA();
        json["skills"] = QString::fromStdString(c.getSkillsAsString());
        json["score"] = c.getScore();
        jsonArray.append(json);
    }

    QJsonDocument doc(jsonArray);
    file.write(doc.toJson());
}

void MainWindow::loadFilteredCandidates() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Filtered List", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray jsonArray = doc.array();

    candidates.clear();
    for (const auto& jsonValue : jsonArray) {
        QJsonObject json = jsonValue.toObject();
        // Extract skills as a QJsonArray and convert to std::vector<std::string>
        QJsonArray skillsJsonArray = json["skills"].toArray();
        std::vector<std::string> skills;
        for (const auto& skillValue : skillsJsonArray) {
            skills.push_back(skillValue.toString().toStdString());
        }

        Candidate c(json["name"].toString().toStdString(),
                    json["university"].toString().toStdString(),
                    json["gpa"].toDouble(),
                    skills,
                    json["hobby"].toString().toStdString(),
                    json["score"].toInt());
        candidates.push_back(c);
    }
    displayCandidates(candidates);
}
