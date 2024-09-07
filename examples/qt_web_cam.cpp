#include "shakhbat_cv.hpp"
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create a main window
    QWidget window;

    // Create a QLabel and set its text
    QLabel *label = new QLabel(&window);
    label->setText("Hello, QLabel!");
    label->setGeometry(60, 60, 10, 10);

    // Adjust the label size to fit the text
    label->adjustSize();

    // Create a QPushButton
    QPushButton *button = new QPushButton(&window);
    button->setText("Push");
    button->setGeometry(0, 0, 50, 50);

    QObject::connect(button, &QPushButton::clicked, label, [label]() {
        label->setText("HI");
    });


    // Set window properties
    window.setWindowTitle("QLabel Example");
    window.resize(200, 100);
    window.show();


    return app.exec();
}

