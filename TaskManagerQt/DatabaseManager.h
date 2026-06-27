#pragma once
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

class DatabaseManager {
public:
	bool open();
	bool createUsersDatabase();
	bool createTasksDatabase();
	~DatabaseManager();
private:
	QSqlDatabase db;
};