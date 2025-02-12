#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    // Initialize the Qt application
    QApplication app(argc, argv);

    // Create the main window and show it
    MainWindow window;
    window.setWindowTitle("Candidate Viewer");
    window.resize(800, 600); // Set an initial size for the window
    window.show();

    // Start the event loop
    return app.exec();
}
