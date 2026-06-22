#include "TaskManagerQt.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    TaskManagerQt window;
    window.setMinimumSize(900, 600);
    window.show();

    return app.exec();
}