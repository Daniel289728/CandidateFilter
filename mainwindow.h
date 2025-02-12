#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <vector>
#include "Candidate.h"
#include "Downloader.h"
#include "Parser.h"
#include "CandidateFilter.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void applyFilters();
    void saveFilteredCandidates();
    void loadFilteredCandidates();
    void rankCandidates();
    void loadCandidates();  // New function to load candidate data

private:
    QTableView *candidateTableView;
    QStandardItemModel *candidateModel;
    QPushButton *filterButton, *saveButton, *loadButton, *rankButton;
    QComboBox *universityComboBox, *skillsComboBox;
    QCheckBox *gpaRangeCheckBox;
    QVBoxLayout *layout;
    QWidget *centralWidget;

    std::vector<Candidate> candidates;  // Store all candidates

    void displayCandidates(const std::vector<Candidate>& candidatesToShow);
};

#endif // MAINWINDOW_H
