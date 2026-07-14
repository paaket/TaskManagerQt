#pragma once
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QSettings>
#include <QVector>
#include "Task.h"
#include "CreateTaskWindow.h"
#include "User.h"
#include "Folder.h"

class DatabaseManager {
public:
	bool open();
	bool createUsersDatabase();
	bool createTasksDatabase();
	bool createFoldersDatabase();
	bool checkLogin(const QString& login);
	QString addNewUser(const QString& login, const QString& password);
	QString logInToAccount(const QString& login, const QString& password, bool remember);
	QVector<Task> findTasksById(int userId);
	QVector<Folder> findFoldersByUserId(int userId);
	QString deleteTaskById(int id);
	QString deleteFolderById(int id);
	QString updateTask(const CreateTaskWindow::TaskData& data, int id);
	QString updadeFolder(const Folder& folder);
	QString markTaskCompleted(int id, int newState);
	QString createTask(const CreateTaskWindow::TaskData& data, int folderId, int userId);
	User findUserDataById(int userId);
	QString updateUser(const User& userUpd);
	QString deleteAccountById(int uderId);
	QString createFolder(const QString& title, int userId);
	~DatabaseManager();
private:
	QSqlDatabase db;
};