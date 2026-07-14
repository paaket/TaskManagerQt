#pragma once
#include <QABstractListModel>
#include <QVector>
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
	void addTask(int id, int user_id, int folder_id, QString title, QString description, int priority, QString deadline, bool completed, QString createdAt);
	QString deleteTask(int id);
	QString editTask(const CreateTaskWindow::TaskData& data, int id);
	QString markCompleted(int id, int newState);
	QString createTask(const CreateTaskWindow::TaskData& data, int currentFolderId);
	User getCurrentUser();
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
private:
	QVector<Task> tasks;
	User user;
	DatabaseManager* dbManager;
};