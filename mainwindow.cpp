#include "MainWindow.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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

    // Populate university and skills combos for demonstration
    universityComboBox->addItem("Polytechnic University of Bucharest");
    universityComboBox->addItem("University of São Paulo");
    universityComboBox->addItem("University of Florida");
    universityComboBox->addItem("University of Havana");

    skillsComboBox->addItem("C++");
    skillsComboBox->addItem("Python");
    skillsComboBox->addItem("Machine Learning");
}

MainWindow::~MainWindow() {}

void MainWindow::applyFilters() {
    // Placeholder for filtering logic
    QMessageBox::information(this, "Filters", "Filters applied!");
}

void MainWindow::saveFilteredCandidates() {
    // Save filtered data to disk
    QString fileName = QFileDialog::getSaveFileName(this, "Save Filtered List", "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            QJsonObject json;
            QJsonArray candidatesArray;
            // Add candidate data as JSON objects
            json["candidates"] = candidatesArray;
            QTextStream stream(&file);
            stream << QJsonDocument(json).toJson();
        }
    }
}

void MainWindow::loadFilteredCandidates() {
    // Load filtered data from disk
    QString fileName = QFileDialog::getOpenFileName(this, "Open Filtered List", "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            // Parse the loaded data and populate candidate model
        }
    }
}

void MainWindow::rankCandidates() {
    // Apply ranking logic based on scoring
    QMessageBox::information(this, "Ranking", "Candidates ranked!");
}
