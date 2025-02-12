#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <vector>
#include <QTextEdit>
#include <QStackedWidget>
#include <QTextStream>
#include "Candidate.h"
#include "Downloader.h"
#include "Parser.h"
#include "candidatescorer.h"
#include "candidatefilter.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadCandidates();
    void displayCandidates(const std::vector<Candidate>& candidatesToShow);
    void applyFilters();
    void saveFilteredCandidates();
    void loadFilteredCandidates();
    void rankCandidates();
    void generateSummary();
    void toggleView();
    void onFilterChanged(const QString& text);

private:
    QTableView *candidateTableView;
    QStandardItemModel *candidateModel;
    QPushButton *filterButton;
    QPushButton *saveButton;
    QPushButton *loadButton;
    QPushButton *rankButton;
    QPushButton *summaryButton;
    QPushButton *toggleViewButton;
    QComboBox *filterComboBox;
    QComboBox *universityComboBox;
    QComboBox *skillsComboBox;
    QLineEdit *gpaRangeInput;
    QLineEdit *scoreInput;
    QLabel *gpaRangeLabel;
    QLabel *scoreLabel;
	QTextEdit* summaryTextEdit;
    QStackedWidget* stackedWidget;
	
    // Layout and central widget
    QVBoxLayout *layout;
    QWidget *centralWidget;

    std::vector<Candidate> candidates;
    std::vector<Candidate> currentlyDisplayedCandidates;
    bool isSummaryView;
};

#endif // MAINWINDOW_H
