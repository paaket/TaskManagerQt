#include "LoginWindow.h"
#include "TaskManagerQt.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSettings>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("taskManager.db");
    if (!db.open()) return 0;

    QSqlQuery query;
    QString queryText;
    queryText = "CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT, login TEXT UNIQUE NOT NULL, password TEXT NOT NULL);";
    if (!query.exec(queryText)) return 0;

    queryText = "CREATE TABLE IF NOT EXISTS tasks(id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER NOT NULL, title TEXT NOT NULL, description TEXT NOT NULL, priority INTEGER NOT NULL, deadline TEXT NOT NULL, completed INTEGER NOT NULL, created_at TEXT NOT NULL, FOREIGN KEY (user_id) REFERENCES users (id));";
    if (!query.exec(queryText)) return 0;

    QSettings settings("Paket", "TaskManagerQt");
    if (!settings.contains("currentUserId")) {
        LoginWindow login;
        if (login.exec() != QDialog::Accepted) return 0;
    }

    TaskManagerQt window;
    window.setMinimumSize(900, 600);
    window.show();

    return app.exec();
}