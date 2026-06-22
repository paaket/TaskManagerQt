#include "TaskManagerQt.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    TaskManagerQt window;
    window.show();
    return app.exec();
}
