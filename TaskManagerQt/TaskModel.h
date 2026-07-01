#pragma once
#include <QABstractListModel>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QModelIndex>
#include <QDebug>
#include "Task.h"
#include "CreateTaskWindow.h"

class TaskModel : public QAbstractListModel {
public:
	enum Roles {
		IdRole = Qt::UserRole,
		UserIdRole,
		TitleRole,
		DescriptionRole,
		PriorityRole,
		DeadlineRole,
		CompletedRole,
		CreatedAtRole
	};
	TaskModel(int userId, QObject* parenr = nullptr);
	void addTask(int id, int user_id, QString title, QString description, int priority, QString deadline, bool completed, QString createdAt);
	void deleteTask(int id);
	void editTask(const CreateTaskWindow::TaskData& data, int id);
	void markCompleted(int id, int newState);
	void createTask(const CreateTaskWindow::TaskData& data);
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
private:
	QVector<Task> tasks;
	int userId;
	QSqlDatabase db;
};