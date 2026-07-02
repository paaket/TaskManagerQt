#include "DatabaseManager.h"
#include "LoginWindow.h"
#include "TaskManagerQt.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSettings>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    DatabaseManager databaseManager;
    if (!databaseManager.open()) return 0;
    
    if (!databaseManager.createUsersDatabase()) return 0;
    if (!databaseManager.createTasksDatabase()) return 0;

    QSettings settings("Paket", "TaskManagerQt");
    if (!settings.contains("currentUserId")) {
        LoginWindow login(&databaseManager);
        if (login.exec() != QDialog::Accepted) return 0;
    }

    TaskManagerQt window(&databaseManager);
    window.setMinimumSize(900, 600);
    window.show();

    return app.exec();
}