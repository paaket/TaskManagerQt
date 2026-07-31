#pragma once
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QSettings>
#include <QVector>
#include <QDateTime>
#include "Task.h"
#include "CreateTaskWindow.h"
#include "User.h"
#include "Folder.h"
#include "Config.h"

class DatabaseManager {
public:
	bool open();
	bool createUsersDatabase();
	bool createTasksDatabase();
	bool createFoldersDatabase();
	bool createTelegramAccountsDatabase();
	bool createTelegramLinkCodesDatabase();
	bool createTaskNotificationsDatabase();
	bool checkLogin(const QString& login);
	QString addNewUser(const QString& login, const QString& password);
	QString logInToAccount(const QString& login, const QString& password, bool remember);
	QVector<Task> findTasksById(int userId);
	QVector<Folder> findFoldersByUserId(int userId);
	QString deleteTaskById(int id);
	QString deleteFolderById(int id);
	QString updateTask(const CreateTaskWindow::TaskData& data, int id);
	QString updateTaskTitle(const QString& title, int id);
	QString updadeFolder(const Folder& folder);
	QString updateTasksFolder(int taskId, int folderId);
	QString markTaskCompleted(int id, int newState);
	QString createTask(const CreateTaskWindow::TaskData& data, int folderId, int userId);
	User findUserDataById(int userId);
	QString updateUser(const User& userUpd);
	QString deleteAccountById(int uderId);
	QString createFolder(const QString& title, int userId);
	bool checkTelegramConnection(int userId);
	QString unlinkTelegramAccount(int userId);
	QString deleteExpiredTelegramCodes();
	QString deleteActiveUserCodes(int userId);
	bool checkTelegramCodeStatus(int code);
	QString addNewTelegramCode(int userId, int code);
	QString checkNotification(int taskId);
	~DatabaseManager();
private:
	QSqlDatabase db;
	Config config;
};