#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QPushButton>
#include <QComboBox>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QWidget>
#include <QCheckBox>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void applyFilters();
    void loadFilteredCandidates();
    void saveFilteredCandidates();
    void rankCandidates();

private:
    QTableView *candidateTableView;
    QStandardItemModel *candidateModel;
    QPushButton *filterButton;
    QPushButton *saveButton;
    QPushButton *loadButton;
    QPushButton *rankButton;
    QComboBox *universityComboBox;
    QComboBox *skillsComboBox;
    QCheckBox *gpaRangeCheckBox;
    QVBoxLayout *layout;
    QWidget *centralWidget;
};

#endif // MAINWINDOW_H
