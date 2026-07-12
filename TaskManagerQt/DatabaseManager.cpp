#include "DatabaseManager.h"

bool DatabaseManager::open() {
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("taskManager.db");
	return db.open();
}

bool DatabaseManager::createUsersDatabase() {
	QSqlQuery query;
	query.exec("PRAGMA foreign_keys = ON;");
	return query.exec("CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT, login TEXT UNIQUE NOT NULL, password TEXT NOT NULL);");
}

bool DatabaseManager::createTasksDatabase() {
	QSqlQuery query;
	return query.exec("CREATE TABLE IF NOT EXISTS tasks(id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER NOT NULL, title TEXT NOT NULL, description TEXT NOT NULL, priority INTEGER NOT NULL, deadline TEXT NOT NULL, completed INTEGER NOT NULL, created_at TEXT NOT NULL, FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE);");
}

bool DatabaseManager::checkLogin(const QString& login) {
	QSqlQuery query;
	if (!query.exec("SELECT login FROM users;")) return false;
	while (query.next()) if (query.value(0) == login) return false;
	return true;
}

QString DatabaseManager::addNewUser(const QString& login, const QString& password) {
	QSqlQuery query;
	query.prepare("INSERT INTO users(login, password) VALUES (:login, :password);");
	query.bindValue(":login", login);
	query.bindValue(":password", password);
	if (!query.exec()) return "insert error: " + query.lastError().text();
	return "";
}

QString DatabaseManager::logInToAccount(const QString& login, const QString& password, bool remember) {
	QSqlQuery query;
	query.prepare("SELECT id, login FROM users WHERE login = :login AND password = :password;");
	query.bindValue(":login", login);
	query.bindValue(":password", password);
	if (!query.exec()) return "select error: " + query.lastError().text();
	while (query.next()) {
		QSettings settings("Paket", "TaskManagerQt");
		settings.setValue("currentUserId", query.value(0).toInt());
		if (remember) settings.setValue("remember", 1);
		else settings.setValue("remember", 0);
		return "true";
	}
	return "false";
}

QVector<Task> DatabaseManager::findTasksById(int userId) {
	QSqlQuery query;
	query.prepare("SELECT * FROM tasks WHERE user_id = :user_id");
	query.bindValue(":user_id", userId);
	if (!query.exec()) return QVector<Task>{};
	QVector<Task> tasks;
	while (query.next()) {
		tasks.append(Task{ query.value(0).toInt(), query.value(1).toInt(), query.value(2).toString(), query.value(3).toString(),
			query.value(4).toInt(), query.value(5).toString(), query.value(6).toBool(), query.value(7).toString() });
	}
	return tasks;
}

QString DatabaseManager::deleteTaskById(int id) {
	QSqlQuery query;
	query.prepare("DELETE FROM tasks WHERE id = :id");
	query.bindValue(":id", id);
	if (!query.exec()) return "delete error: " + query.lastError().text();
	return "";
}

QString DatabaseManager::updateTask(const CreateTaskWindow::TaskData& data, int id) {
	QSqlQuery query;
	query.prepare("UPDATE tasks SET title = :title, description = :description, priority = :priority, deadline = :deadline WHERE id = :id;");
	query.bindValue(":title", data.title);
	query.bindValue(":description", data.description);
	query.bindValue(":priority", data.priority);
	query.bindValue(":deadline", data.deadline);
	query.bindValue(":id", id);
	if (!query.exec()) return "update error: " + query.lastError().text();
	return "";
}

QString DatabaseManager::markTaskCompleted(int id, int newState) {
	QSqlQuery query;
	query.prepare("UPDATE tasks SET completed = :completed WHERE id = :id");
	query.bindValue(":completed", newState);
	query.bindValue(":id", id);
	if (!query.exec()) return "update error: " + query.lastError().text();
	return "";
}

QString DatabaseManager::createTask(const CreateTaskWindow::TaskData& data, int userId) {
	QSqlQuery query;
	query.prepare("INSERT INTO tasks(user_id, title, description, priority, deadline, completed, created_at) VALUES (:user_id, :title, :description, :priority, :deadline, :completed, :created_at)");
	query.bindValue(":user_id", userId);
	query.bindValue(":title", data.title);
	query.bindValue(":description", data.description);
	query.bindValue(":priority", data.priority);
	query.bindValue(":deadline", data.deadline);
	query.bindValue(":completed", 0);
	query.bindValue(":created_at", data.createdAt);
	if (!query.exec()) return "insert error: " + query.lastError().text();
	return query.lastInsertId().toString();
}

User DatabaseManager::findUserDataById(int userId) {
	User user;
	QSqlQuery query;
	query.prepare("SELECT login, password FROM users WHERE id = :id");
	query.bindValue(":id", userId);
	if (!query.exec()) return user;
	if (query.next()) {
		user.id = userId;
		user.login = query.value(0).toString();
		user.password = query.value(1).toString();
	}
	return user;
}

QString DatabaseManager::updateUser(const User& userUpd) {
	QSqlQuery query;
	query.prepare("UPDATE users SET login = :login, password = :password WHERE id = :id;");
	query.bindValue(":login", userUpd.login);
	query.bindValue(":password", userUpd.password);
	query.bindValue(":id", userUpd.id);
	if (!query.exec()) return "update error: " + query.lastError().text();
	return "";
}

QString DatabaseManager::deleteAccountById(int userId) {
	QSqlQuery query;
	query.prepare("DELETE FROM users WHERE id = :id;");
	query.bindValue(":id", userId);
	if (!query.exec()) return "delete error: " + query.lastError().text();
	return "";
}

DatabaseManager::~DatabaseManager() {
	db.close(); 
};