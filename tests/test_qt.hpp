#ifndef SBROWSER_TEST_QT_HPP
#define SBROWSER_TEST_QT_HPP


#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>


void test_qt(int argc, char** argv) {
    QApplication app{argc, argv};
    auto* window = new QWidget{};
    auto* button = new QPushButton{window};
    button->setText("Hello World");

    window->showMaximized();
    QApplication::exec();
}


#endif //SBROWSER_TEST_QT_HPP
