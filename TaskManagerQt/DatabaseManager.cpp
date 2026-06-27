#include "DatabaseManager.h"

bool DatabaseManager::open() {
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("taskManager.db");
	return db.open();
}

bool DatabaseManager::createUsersDatabase() {
	QSqlQuery query;
	return query.exec("CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT, login TEXT UNIQUE NOT NULL, password TEXT NOT NULL);");
}

bool DatabaseManager::createTasksDatabase() {
	QSqlQuery query;
	return query.exec("CREATE TABLE IF NOT EXISTS tasks(id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER NOT NULL, title TEXT NOT NULL, description TEXT NOT NULL, priority INTEGER NOT NULL, deadline TEXT NOT NULL, completed INTEGER NOT NULL, created_at TEXT NOT NULL, FOREIGN KEY (user_id) REFERENCES users (id));");
}

DatabaseManager::~DatabaseManager() { db.close(); };