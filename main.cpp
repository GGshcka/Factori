#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow window;
    window.showFullScreen();

    return QApplication::exec();
}
