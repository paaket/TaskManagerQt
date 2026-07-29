#pragma once
#include <QABstractListModel>
#include <QVector>
#include <QDateTime>
#include <QModelIndex>
#include "Task.h"
#include "CreateTaskWindow.h"
#include "DatabaseManager.h"
#include "User.h"

class TaskModel : public QAbstractListModel {
public:
	enum Roles {
		IdRole = Qt::UserRole,
		UserIdRole,
		FolderIdRole,
		TitleRole,
		DescriptionRole,
		PriorityRole,
		DeadlineRole,
		CompletedRole,
		CreatedAtRole
	};
	TaskModel(int userId, DatabaseManager* dbManager, QObject* parenr = nullptr);
	void addTask(int id, int user_id, int folder_id, const QString& title, const QString& description, int priority, QDateTime deadline, bool completed, QDateTime createdAt);
	QString deleteTask(int id);
	QString editTask(const CreateTaskWindow::TaskData& data, int id);
	QString markCompleted(int id, int newState);
	QString createTask(const CreateTaskWindow::TaskData& data, int currentFolderId);
	QString changeTasksFolder(int taskId, int folderId);
	User getCurrentUser();
	QVector<Folder> getFolders();
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

private:
	QVector<Task> tasks;
	User user;
	DatabaseManager* dbManager;
};