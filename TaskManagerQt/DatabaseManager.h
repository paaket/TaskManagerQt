#pragma once
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QSettings>
#include <QVector>
#include "Task.h"
#include "CreateTaskWindow.h"

class DatabaseManager {
public:
	bool open();
	bool createUsersDatabase();
	bool createTasksDatabase();
	bool checkLogin(const QString& login);
	QString addNewUser(const QString& login, const QString& password);
	QString logInToAccount(const QString& login, const QString& password, bool remember);
	QVector<Task> findTasksById(int userId);
	QString deleteTaskBtId(int id);
	QString updateTask(const CreateTaskWindow::TaskData& data, int id);
	QString markTaskCompleted(int id, int newState);
	QString createTask(const CreateTaskWindow::TaskData& data, int userId);
	~DatabaseManager();
private:
	QSqlDatabase db;
};